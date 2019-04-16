# BitmapHandler(图像处理)
## 内容说明
使用C++语言编写的图像处理工具，输入为图片，输出也是图片（处理起来也能会稍微慢一点，以后可能会使用缓存以及图形化界面），目前包含了以下功能：
1. 24位真彩色位图信息读取并存储
2. 24位真彩色位图三元色分离
3. 24位真彩色位图灰度化为8位
4. 8位灰度图反色
5. 8位灰度图的直方图
6. 8为灰度图均衡化
## 使用
* 环境：Visual Studio（我用的是2017）<br />
* 克隆：将本仓库代码克隆到你的电脑上<br />
* 打开：将目录切换到项目根目录，双击打开BitmapHandler.sln文件<br />
* 运行：将要处理的图片放在"/BitmapHandler/bitmaps"文件夹下，并修改main.cpp里面的文件路径，运行本地windows调试器即可开始处理你想要处理的图片（本项目已经自带几张图片，你可以直接运行测试）<br />