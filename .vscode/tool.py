# %%
# 获取所有的头文件路径
import os
import sys
P = []
for root, dirs, files in os.walk(".."):
    for name in files:
        if name.endswith(".h") or name.endswith(".hpp"):
            if root not in sys.path:
                p = os.path.join(root)
                p = p.replace("\\", "/")
                p = '"' + "${workspaceFolder}" + p[2:] + '"' + ","
                if p not in P:
                    P.append(p)
                    print(p)

# %%
print("FF "*1000)

# %%
# gpio定义转换
# 读取文件
f = open("../Core/Inc/main.h", "r")
main_h_str = f.read()
f.close()

ind = []
main_h_list = main_h_str.split('\n')
for each_line in main_h_list:
    if "Private defines" in each_line:
        ind.append(main_h_list.index(each_line))
gpio_list = main_h_list[ind[0]+1:ind[1]-1]


port = gpio_list[1::2]
pin = gpio_list[0::2]


for i in range(len(pin)):
    port_ = port[i].split(' ')[1]
    pin_ = pin[i].split(' ')[1]
    # {{port, pin}, false}, //
    print('{{' + f"{port_}, {pin_}" + '}, false}, //')
# %%
f = open("../App/Src/drv_gpio.c", "r", encoding="utf-8")
drv_gpio_c = f.read()
f.close()
drv_gpio_c = drv_gpio_c.split('\n')

gpioSignal = []
for i in range(len(pin)):
    port_ = port[i].split(' ')[1]
    pin_ = pin[i].split(' ')[1]
    # print(f"{{{port_}, {pin_}, false}}, //")

    for each_line in drv_gpio_c:
        if (port_ in each_line and pin_ in each_line):
            if ("true" in each_line in each_line):
                gpioSignal.append('{{' + f"{port_}, {pin_}" + '}, true}, // r')
            else:
                gpioSignal.append('{{' + f"{port_}, {pin_}" + '}, false}, //')


start_line = 0
end_line = 0
for each_line in drv_gpio_c:
    if "gpioSignal" in each_line:
        start_line = drv_gpio_c.index(each_line)
    if "};" in each_line:
        end_line = drv_gpio_c.index(each_line)
        break

txt = ""
for each in drv_gpio_c[:start_line+1]:
    txt += each + "\n"
for each in gpioSignal:
    txt += each + "\n"
for each in drv_gpio_c[end_line:]:
    txt += each + "\n"

# 去掉最后多余的\n
while txt[-1] == "\n":
    txt = txt[:-1]
txt += "\n"
# print(txt)
# 写入文件
f = open("../App/Src/drv_gpio.c", "w", encoding="utf-8")
f.write(txt)
f.close()

#%%
# 绘制 -（81-（LQI*91)/255） 图像
import matplotlib.pyplot as plt
import numpy as np
LQI = np.arange(0, 256, 1)
RSSI = -(81 - (LQI*91)/255)
plt.plot(LQI, RSSI)

