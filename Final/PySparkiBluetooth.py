 coding: utf-8

# In[1]:


import serial


# In[2]:


def init(com_port,baudrate=9600):
    self = serial.Serial(str(com_port),int(9600),write_timeout = 1, timeout = 1)

def close():
    self.write('1,000,000n')


# In[3]:


def release():
    self.write('2,000,000n')


# In[4]:


def move(map_index):
    if map_index < 10:
        map_index = "00"+str(map_index)
    elif map_index>9 and map_index<100:
        map_index = "0"+str(map_index)
    else:
        map_index = str(map_index)
    print map_index
    self.write('3,'+map_index+',000n')


# In[5]:


def update_pos(map_index,theta):
    self.write('4,'+str(map_index)+','+str(theta)+'n')
    


# In[12]:


def reset():
    self.write('r')