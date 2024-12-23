#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#    include <vsgXchange/all.h>
#endif

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <vsgQt6/Widget.h>

#include <iostream>

vsgQt6::Widget* createWidget(vsg::ref_ptr<vsgQt6::Viewer> viewer, vsg::ref_ptr<vsg::WindowTraits> traits, vsg::ref_ptr<vsg::Node> vsg_scene, QWidget* parent, const QString& title = {})
{
    auto widget = new vsgQt6::Widget(viewer, parent);

    widget->initialize();

    // if this is the first window to be created, use its device for future window creation.
    if (!traits->device) traits->device = widget->windowAdapter->getOrCreateDevice();

    // compute the bounds of the scene graph to help position camera
    vsg::ComputeBounds computeBounds;
    vsg_scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
    double nearFarRatio = 0.001;

    uint32_t width = widget->width();
    uint32_t height = widget->height();

    vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel(vsg_scene->getObject<vsg::EllipsoidModel>("EllipsoidModel"));
    vsg::ref_ptr<vsg::Camera> camera;
    {
        // set up the camera
        auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

        vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
        if (ellipsoidModel)
        {
            perspective = vsg::EllipsoidPerspective::create(
                lookAt, ellipsoidModel, 30.0,
                static_cast<double>(width) /
                    static_cast<double>(height),
                nearFarRatio, false);
        }
        else
        {
            perspective = vsg::Perspective::create(
                30.0,
                static_cast<double>(width) /
                    static_cast<double>(height),
                nearFarRatio * radius, radius * 4.5);
        }

        camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{width, height}));
    }

    auto trackball = vsg::Trackball::create(camera, ellipsoidModel);
    trackball->addWindow(*widget);

    viewer->addEventHandler(trackball);

    auto commandGraph = vsg::createCommandGraphForView(*widget, camera, vsg_scene);

    viewer->addRecordAndSubmitTaskAndPresentation({commandGraph});

    return widget;
}

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    vsg::CommandLine arguments(&argc, argv);

    // set up vsg::Options to pass in filepaths, ReaderWriters and other IO
    // related options to use when reading and writing files.
    auto options = vsg::Options::create();
    options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
    options->paths = vsg::getEnvPaths("VSG_FILE_PATH");
#ifdef vsgXchange_FOUND
    options->add(vsgXchange::all::create());
#endif

    arguments.read(options);

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "vsgQt6 widgets";
    windowTraits->debugLayer = arguments.read({"--debug", "-d"});
    windowTraits->apiDumpLayer = arguments.read({"--api", "-a"});
    arguments.read("--samples", windowTraits->samples);
    arguments.read({"--window", "-w"}, windowTraits->width, windowTraits->height);
    if (arguments.read({"--fullscreen", "--fs"})) windowTraits->fullscreen = true;

    bool continuousUpdate = !arguments.read({"--event-driven", "--ed"});
    auto interval = arguments.value<int>(8, "--interval");

    if (arguments.errors())
        return arguments.writeErrorMessages(std::cerr);

    if (argc <= 1)
    {
        std::cout << "Please specify a 3d model or image file on the command line."
                  << std::endl;
        return 1;
    }

    vsg::Path filename = arguments[1];

    auto vsg_scene = vsg::read_cast<vsg::Node>(filename, options);
    if (!vsg_scene)
    {
        std::cout << "Failed to load a valid scene graph. Please specify a valid 3d "
                     "model or image file on the command line."
                  << std::endl;
        return 1;
    }


    QMainWindow* mainWindow = new QMainWindow();

    // create the viewer that will manage all the rendering of the views
    auto viewer = vsgQt6::Viewer::create();

    auto widget = createWidget(viewer, windowTraits, vsg_scene, nullptr, "First Widget");

    mainWindow->setCentralWidget(widget);

    mainWindow->setGeometry(windowTraits->x, windowTraits->y, windowTraits->width, windowTraits->height);

    mainWindow->show();

    if (interval >= 0) viewer->setInterval(interval);
    viewer->continuousUpdate = continuousUpdate;

    viewer->addEventHandler(vsg::CloseHandler::create(viewer));
    viewer->compile();

    return application.exec();
}
