#!/usr/bin/env python
# coding: utf-8

# In[1]:


import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision
from torchvision.datasets import MNIST
# Note: this example requires the torchmetrics library: https://torchmetrics.readthedocs.io
import torchmetrics
import numpy as np


# In[2]:


import torchhd
from torchhd.models import Centroid
from torchhd import embeddings


# In[3]:


from tqdm import tqdm


# Use the GPU if available

# In[4]:


device = torch.device("cuda")
print("Using {} device".format(device))


# Debug options

# In[5]:


torch.set_printoptions(profile="full")


# In[6]:


DIMENSIONS = 2048
IMG_SIZE = 28
NUM_LEVELS = 16
BATCH_SIZE = 10  # for GPUs with enough memory we can process multiple images at ones


# In[7]:


transform = torchvision.transforms.ToTensor()


# In[8]:


train_ds = MNIST("../data", train=True, transform=transform, download=True)
train_ld = torch.utils.data.DataLoader(
    train_ds, batch_size=BATCH_SIZE, shuffle=True)


# In[9]:


test_ds = MNIST("../data", train=False, transform=transform, download=True)
test_ld = torch.utils.data.DataLoader(
    test_ds, batch_size=BATCH_SIZE, shuffle=False)


# In[10]:


class Encoder(nn.Module):
    def __init__(self, out_features, size, levels):
        super(Encoder, self).__init__()
        self.flatten = torch.nn.Flatten()
        self.position = embeddings.Random(size * size, out_features, "BSC", dtype=torch.int32)
        self.value = embeddings.Level(levels, out_features, "BSC", dtype=torch.int32)
        rotate = self.position.weight[0]
        for i in range(1, size * size):
            rotate = torch.roll(rotate, 1)
            self.position.weight[i] = rotate
        
    def forward(self, x):
        x = self.flatten(x)
        sample_hv = torchhd.bind(self.position.weight, self.value(x))
        sample_hv = torchhd.multiset(sample_hv)
        return sample_hv


# In[11]:


encode = Encoder(DIMENSIONS, IMG_SIZE, NUM_LEVELS)
encode = encode.to(device)


# In[12]:


num_classes = len(train_ds.classes)
model = Centroid(DIMENSIONS, num_classes, dtype=torch.int32)
model = model.to(device)


# In[13]:


with torch.no_grad():
    for samples, labels in tqdm(train_ld, desc="Training"):
        samples = samples.to(device)
        labels = labels.to(device)
        samples_hv = encode(samples)
        model.add(samples_hv, labels)


# In[14]:


accuracy = torchmetrics.Accuracy("multiclass", num_classes=num_classes)
print(model.weight[0])


# In[15]:


with torch.no_grad():
    for i, tensor in enumerate(model.weight):
        # Majority Voting
        tensor = tensor > 3000
        model.weight[i] = torchhd.ensure_vsa_tensor(tensor, "BSC", dtype=torch.bool)
    model.dtype = torch.bool
    for samples, labels in tqdm(test_ld, desc="Testing"):
        samples = samples.to(device)
        samples_hv = encode(samples)
        samples_hv = torchhd.ensure_vsa_tensor(samples_hv, "MAP")
        samples_hv = samples_hv.to(torch.bool)
        # print(samples_hv)
        outputs = model(samples_hv)
        accuracy.update(outputs.cpu(), labels)


# In[16]:


print(f"Testing accuracy of {(accuracy.compute().item() * 100):.3f}%")


# In[17]:


level_hv = encode.value.weight
position_hv = encode.position.weight
sample_hv = model.weight


# In[18]:


print(type(level_hv[0]))
print(type(position_hv[0]))
print(type(sample_hv[0]))


# In[36]:


def hv2c(tensors, name):
    file = open("hv.c", 'a')
    vhex = np.vectorize(hex)
    file.write(f"const unsigned char {name}[][{DIMENSIONS // 8}] = {{\n")
    for i, tensor in enumerate(tensors):
        numpy_array = tensor.cpu().numpy()
        bit_arr = np.packbits(numpy_array)
        
        file.write(f"[{i}] = {{")
        for byte in vhex(bit_arr):
            file.write(byte + ',')
        file.write("},\n")
    file.write("};\n")
        

hv2c(level_hv, "level_hypervectors")
hv2c(position_hv, "position_hypervectors")
hv2c(sample_hv, "sample_hypervectors")


# In[39]:


level = torchhd.ensure_vsa_tensor(level_hv[0], "MAP")
print(torchhd.cosine_similarity(level, sample_hv[0]))


# In[ ]:




