#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2024 Nolan Kramer

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsgQt6/Export.h>

#include <vsg/app/Window.h>

#include <QQuickWindow>

namespace vsgQt6
{

    // Uses QVulkanInstance::surfaceForWindow() to retrieve the surface.
    class VSGQT6_DECLSPEC Window_QQuickWindow : public vsg::Inherit<vsg::Window, Window_QQuickWindow>
    {
        public:
            Window_QQuickWindow(QQuickWindow* quickWindow, vsg::ref_ptr<vsg::WindowTraits> in_traits);
            ~Window_QQuickWindow();

            virtual const char* instanceExtensionSurfaceName() const override { return "VK_KHR_QQuickWindow_surface"; }

            QQuickWindow* getQuickWindow() { return _quickWindow; }

        protected:
            virtual void _initSurface() override;

        private:
            QQuickWindow* _quickWindow;
    };

}
