#include <vsgQt6/Window_QQuickWindow.h>

#include <QVulkanInstance>

using namespace vsgQt6;

Window_QQuickWindow::Window_QQuickWindow(QQuickWindow* quickWindow, vsg::ref_ptr<vsg::WindowTraits> in_traits)
    : vsg::Inherit<vsg::Window, Window_QQuickWindow>(in_traits), _quickWindow(quickWindow)
{ }

Window_QQuickWindow::~Window_QQuickWindow()
{ }

void Window_QQuickWindow::_initSurface()
{
    if (!_instance) _initInstance();

    // Retrieve the Vulkan surface for the QQuickWindow
    VkSurfaceKHR surface = QVulkanInstance::surfaceForWindow(_quickWindow);
    _surface = vsg::Surface::create(surface, _instance);
}
