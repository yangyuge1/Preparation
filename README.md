# Preparation
Preparation for the National College Student Electronic Design Competition  
本仓库内容为2023年电赛E题**未整理**原始代码  
视频参见个人B站主页置顶：https://space.bilibili.com/666680560  
## 硬件清单
### 绿色激光云台
步进电机x2  
电机驱动x2: 张大头 ZDT_X42 步进闭环驱动模块，工作在开环脉冲控制模式  
绿色激光笔x1  
Nvidia_Jetson_Nano x1  
摄像头x1  
TM4C123GH6PM x1  
集成供电模块底板 x1 ：包含供电、光耦隔离、按键  
铝合金导轨x1  
12V锂电池  
支架、轴承、3D打印配件若干  
### 红色激光云台
步进电机x2  
电机驱动x2: 张大头 Emm_V4.2步进闭环驱动模块，工作在闭环脉冲控制模式  
绿色激光笔x1  
Nvidia_Jetson_Nano x1  
摄像头x1  
TM4C123GH6PM x1  
集成供电模块底板 x1  
12V锂电池<br>
支架、轴承、3D打印配件若干  
### 公共部分
铝合金摄像头支架  
白色背景版  
木质底座  
## 软件清单
### Nvidia_Jetson_Nano/test_rectangle.py
视觉部分程序，部署属于Nvidia_Jetson_Nano，因为视觉部分的硬件基本一致，所以共享同一套代码，仅初始参数不同  
### TM4C123GH6PM/big_two_axis_gimbal
红色激光云台控制部分，部署于TM4C123GH6PM  
### TM4C123GH6PM/big_two_axis_gimbal_green
绿色激光云台控制部分，部署于TM4C123GH6PM  
控制部分开发环境为keil，非ccs
### image
作品图片  
### Modules
电赛之前整理的模块
Modules  
|--ATK-MWCC68D 正点原子无线串口模块驱动程序  
|--Cmplex_task_support 复杂任务流程支持  
|        |--BinaryTree 二叉树  
|        |--Command 基于union数据转换  
|        |--Controller pid控制器  
|        |--Graph 图  
|        |--LinkedList 链表  
|        |--MemoryPool 动态内存空间管理  
|        |--complex_task_support.c 延时与回调函数   
|--EEPROM 数据持久化  
|--Encoder 编码器  
|--IIC  
|--JY901B 维特智能陀螺仪模块  
|--SSI  
|--Stepper_motor 步进电机串口控制  
|--UART  
|--US-100 超声波测距  
|--ZDT_X42 张大头步进电机软件脉冲控制  
