import os
import shutil
os.chdir('/media/influx/Programming/Projects/project-4-InFluX-M/EnglishData')

i = 1
ls = os.listdir()
for c in ls:
    os.rename(c, str(i) + '.txt')   
    i+=1 

