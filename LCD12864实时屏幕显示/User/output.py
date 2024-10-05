'''
作者：qILoongIp
此处代码的作用为捕获屏幕，二值化处理，去字模并通过串口发送
'''
import serial
import time
import numpy as np
from PIL import ImageGrab, Image
import cv2
import threading

# 打开串口
ser = serial.Serial('COM5', 115200, timeout=1)#串口设置自行调整

def capture_and_process():
    try:
        while True:
            # 截取屏幕内容
            screen = ImageGrab.grab(bbox=(0, 0, 2560, 1440))#屏幕大小自行调整

            # 转换为128x64的大小
            screen_resized = screen.resize((128, 64), Image.ANTIALIAS)

            # 转换为灰度图像
            screen_gray = screen_resized.convert('L')

            # 二值化处理
            threshold = 200  # 阈值，可以根据需要调整
            screen_binary = screen_gray.point(lambda p: p > threshold and 255)

            # 显示处理前的图像
            cv2.imshow('Original Image', cv2.resize(np.array(screen), (512, 288)))

            # 显示处理后的图像
            cv2.imshow('Binary Image', cv2.resize(np.array(screen_binary), (512, 288), interpolation=cv2.INTER_NEAREST))

            # 检测键盘输入，如果按下'q'键则退出循环
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    finally:
        cv2.destroyAllWindows()

def send_data():
    try:
        while True:
            # 截取屏幕内容
            screen = ImageGrab.grab(bbox=(0, 0, 2560, 1440))

            # 转换为128x64的大小
            screen_resized = screen.resize((128, 64), Image.ANTIALIAS)

            # 转换为灰度图像
            screen_gray = screen_resized.convert('L')

            # 二值化处理
            threshold = 200  # 阈值，可以根据需要调整
            screen_binary = screen_gray.point(lambda p: p > threshold and 255)

            # 将图像转换为数组
            screen_array = np.array(screen_binary)

            # 取反操作
            screen_array = np.invert(screen_array)

            # 压缩数组
            compressed_array = np.packbits(screen_array)

            # 将图像数据转换为16x64个字节
            image_bytes = [compressed_array[i:i+16] for i in range(0, len(compressed_array), 16)]
            # 发送图像数据到串口
            for row in image_bytes:
                ser.write(row)  # 将十六进制字符串转换为字节并发送到串口

    finally:
        # 关闭串口
        ser.close()

# 创建并启动线程
capture_thread = threading.Thread(target=capture_and_process)
send_thread = threading.Thread(target=send_data)

capture_thread.start()
send_thread.start()
