#include <vsgQt6/Widget.h>

#include <QPlatformSurfaceEvent>

using namespace vsgQt6;

Widget::Widget(vsg::ref_ptr<Viewer> in_viewer, QWidget* parent)
    : QQuickWidget(parent), viewer(in_viewer)
{
    // Constructor implementation
}

Widget::~Widget()
{
    cleanup();
}

void Widget::initialize()
{
    if (windowAdapter) return;

    if (!traits) traits = vsg::WindowTraits::create();

    traits->x = convert_coord(x());
    traits->y = convert_coord(y());
    traits->width = convert_coord(width());
    traits->height = convert_coord(height());

    windowAdapter = Window_QQuickWindow::create(quickWindow(), traits);

    _initialized = true;
}

void Widget::cleanup()
{
    viewer = {};
}

bool Widget::event(QEvent* e)
{
    switch (e->type())
    {
    case QEvent::PlatformSurface: {
        auto surfaceEvent = dynamic_cast<QPlatformSurfaceEvent*>(e);
        if (surfaceEvent->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed)
        {
            vsg::clock::time_point event_time = vsg::clock::now();
            windowAdapter->bufferedEvents.push_back(vsg::CloseWindowEvent::create(windowAdapter, event_time));

            cleanup();
        }
        break;
    }

    default:
        break;
    }

    return QWidget::event(e);
}

void Widget::hideEvent(QHideEvent* /*e*/)
{
}

void Widget::resizeEvent(QResizeEvent* /*e*/)
{
    if (!windowAdapter) return;

    vsg::clock::time_point event_time = vsg::clock::now();
    windowAdapter->bufferedEvents.push_back(vsg::ConfigureWindowEvent::create(windowAdapter, event_time, convert_coord(x()), convert_coord(y()), convert_coord(width()), convert_coord(height())));

    windowAdapter->resize();

    if (viewer) viewer->request();
}

void Widget::keyPressEvent(QKeyEvent* e)
{
    if (!windowAdapter) return;

    vsg::KeySymbol keySymbol, modifiedKeySymbol;
    vsg::KeyModifier keyModifier;

    if (keyboardMap->getKeySymbol(e, keySymbol, modifiedKeySymbol, keyModifier))
    {
        vsg::clock::time_point event_time = vsg::clock::now();
        windowAdapter->bufferedEvents.push_back(vsg::KeyPressEvent::create(windowAdapter, event_time, keySymbol, modifiedKeySymbol, keyModifier));
    }

    if (viewer) viewer->request();
}

void Widget::keyReleaseEvent(QKeyEvent* e)
{
    if (!windowAdapter) return;

    vsg::KeySymbol keySymbol, modifiedKeySymbol;
    vsg::KeyModifier keyModifier;

    if (keyboardMap->getKeySymbol(e, keySymbol, modifiedKeySymbol, keyModifier))
    {
        vsg::clock::time_point event_time = vsg::clock::now();
        windowAdapter->bufferedEvents.push_back(vsg::KeyReleaseEvent::create(windowAdapter, event_time, keySymbol, modifiedKeySymbol, keyModifier));
    }

    if (viewer) viewer->request();
}

void Widget::mouseMoveEvent(QMouseEvent* e)
{
    if (!windowAdapter) return;

    vsg::clock::time_point event_time = vsg::clock::now();

    auto [mask, button] = convertMouseButtons(e);
    auto [x, y] = convertMousePosition(e);

    windowAdapter->bufferedEvents.push_back(vsg::MoveEvent::create(windowAdapter, event_time, x, y, mask));

    if (viewer) viewer->request();
}

void Widget::mousePressEvent(QMouseEvent* e)
{
    if (!windowAdapter) return;

    vsg::clock::time_point event_time = vsg::clock::now();

    auto [mask, button] = convertMouseButtons(e);
    auto [x, y] = convertMousePosition(e);

    windowAdapter->bufferedEvents.push_back(vsg::ButtonPressEvent::create(windowAdapter, event_time, x, y, mask, button));

    if (viewer) viewer->request();
}

void Widget::mouseReleaseEvent(QMouseEvent* e)
{
    if (!windowAdapter) return;

    vsg::clock::time_point event_time = vsg::clock::now();

    auto [mask, button] = convertMouseButtons(e);
    auto [x, y] = convertMousePosition(e);

    windowAdapter->bufferedEvents.push_back(vsg::ButtonReleaseEvent::create(windowAdapter, event_time, x, y, mask, button));

    if (viewer) viewer->request();
}

void Widget::wheelEvent(QWheelEvent* e)
{
    if (!windowAdapter) return;

    vsg::clock::time_point event_time = vsg::clock::now();
    windowAdapter->bufferedEvents.push_back(vsg::ScrollWheelEvent::create(windowAdapter, event_time, e->angleDelta().y() < 0 ? vsg::vec3(0.0f, -1.0f, 0.0f) : vsg::vec3(0.0f, 1.0f, 0.0f)));

    if (viewer) viewer->request();
}

std::pair<vsg::ButtonMask, uint32_t> Widget::convertMouseButtons(QMouseEvent* e) const
{
    uint16_t mask{0};
    uint32_t button = 0;

    if (e->buttons() & Qt::LeftButton) mask = mask | vsg::BUTTON_MASK_1;
    if (e->buttons() & Qt::MiddleButton) mask = mask | vsg::BUTTON_MASK_2;
    if (e->buttons() & Qt::RightButton) mask = mask | vsg::BUTTON_MASK_3;

    switch (e->button())
    {
    case Qt::LeftButton: button = 1; break;
    case Qt::MiddleButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    default: break;
    }

    return {static_cast<vsg::ButtonMask>(mask), button};
}

std::pair<int32_t, int32_t> Widget::convertMousePosition(QMouseEvent* e) const
{
#if QT_VERSION_MAJOR == 6
    return {convert_coord(e->position().x()), convert_coord(e->position().y())};
#else
    return {convert_coord(e->x()), convert_coord(e->y())};
#endif
}

