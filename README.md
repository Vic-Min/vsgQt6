# vsgQt6
Open Source ([MIT Licensed](LICENSE.md)), cross platform C++ library providing integration of VulkanSceneGraph with Qt windowing. Supports Windows, Linux and macOS.

vsgQt6 provides full Vulkan support through the VulkanSceneGraph's built in VkSurface support rather than the limited Vulkan support that Qt-5.10 or later provide. Using the VulkanSceneGraph for providing Vulkan support avoids the restriction that Qt's VulkanWindow has with not being able to share VkDevice between windows, and provides compatibility with Qt versions prior to it adding Vulkan support.  Sharing vsg::Device/VkDevice between widgets is crucial for providing multiple 3D viewport widgets without blowing up GPU memory usage.

vsgQt6 provides native Qt integration through the use of a QQuickWidget or QQuickWindow. It does this in the follow manner:
1. Set the QQuickWindow graphics API to Vulkan with `QQuickWindow::setGraphicsApi`
2. Create a Qt graphics device **from our vkDevice** with `QQuickGraphicsDevice::fromDeviceObjects`
3. Set the QQuickWindow graphics device with the above created graphics device
4. Subclass `QQuickWidget` and have it create a `QSGTexture` that in-turn delivers a `QRhiTexture`. The underlying texture buffer can be accessed with `QRhiTexture::nativeTexture()`
5. VSG renders into the above buffer
6. The `QQuickWidget` renders a scene with a single `QSGSimpleTextureNode`, which ultimately renders the `QSGTexture` that is "shared" with VSG
7. There are various events that can be used to integrate the render loop, such as `QQuickItem::updatePaintNode` or `QQuickWindow::beforeRendering`

## Checking out vsgQt6

~~~ sh
git clone https://github.com/vsg-dev/vsgQt6.git
~~~

## Dependencies:

* [VulkanSDK](https://www.lunarg.com/vulkan-sdk/) version 1.1.70 or later
* [VulkanSceneGraph](https://github.com/vsg-dev/VulkanSceneGraph) master, 1.0.8 or later.
* [CMake](https://cmake.org/) version 3.10 or later
* [Qt](https://www.qt.io/) version 6 or later
* C++17 capable compiler

## Building vsgQt6

~~~ sh
cd vsgQt6
mkdir build && cd build
cmake ..
cmake --build . -j8
~~~

## Examples

~~~ sh
./bin/vsgqtviewer <3D MODEL FILEPATH>
./bin/vsgqtmultiviewer <3D MODEL FILEPATH>
./bin/vsgqtwindows <3D MODEL FILEPATH>
~~~

* [vsgqtviewer](examples/vsgqtviewer/main.cpp) - example of single QApplication/QMainWindow usage with single vsgQt6::Viewer.
* [vsgqtmultiviewer](examples/vsgqtmultiviewer/main.cpp) - example of QWindow usage with multiple vsgQt6::Viewer in the same QWindow.
* [vsgqtwindows](examples/vsgqtwindows/main.cpp) - example of multiple vsgQt6::Viewer per QWindow.
