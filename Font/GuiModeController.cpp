#include "GuiModeController.h"
#include "JsonParserException.h"
#include <QtWidgets/QMessageBox>
#include "MainWindow.h"
#include <QtWidgets/QVBoxLayout>


GuiModeController::GuiModeController()
{
}



GuiModeController::~GuiModeController()
{
	delete config;
	delete serializer;
	delete parser;
	delete mainWindow;
}

void GuiModeController::run()
{
	parser = new saveAndLoadConfig::ConfigParser();
	try
	{
        QList<Point*> defaultPointsList;

        defaultPointsList.append(new Point(30, 60, true));
        defaultPointsList.append(new Point(0, 0, false));
        defaultPointsList.append(new Point(40, 60, true));
        defaultPointsList.append(new Point(40, 30, true));
        defaultPointsList.append(new Point(30, 30, true));

//        defaultPointsList.append(new Point(0, 0, true));
//        defaultPointsList.append(new Point(10, 50, false));
//        defaultPointsList.append(new Point(30, 10, true));
//        defaultPointsList.append(new Point(20, 10, true));
//        defaultPointsList.append(new Point(40, 50, false));
//        defaultPointsList.append(new Point(50, 0, true));


//        defaultPointsList.append(new Point(0, 0, true));
//        defaultPointsList.append(new Point(10, 0, true));
//        defaultPointsList.append(new Point(20, -40, false));
//        defaultPointsList.append(new Point(30, 0, true));
//        defaultPointsList.append(new Point(40, 0, true));
//        defaultPointsList.append(new Point(20, -60, false));


        Figure* defaultFigure= new Figure(defaultPointsList);
        config = new Configuration(defaultFigure, new Panel(DEFAULT_HEIGHT, DEFAULT_WIDTH),
                                   DEFAULT_X, DEFAULT_Y,
                                   DEFAULT_SCALE, DEFAULT_FILL, DEFAULT_OUTLINE);
		serializer = new saveAndLoadConfig::Serializer();

        drawPanel = new DrawPanel(config, this);
		QGroupBox* controlsPanel;
		initControllersPanel(controlsPanel);

		mainWindow = new MainWindow(drawPanel, controlsPanel);

		connect(mainWindow, SIGNAL(open(QString)), SLOT(open(QString)));
		connect(mainWindow, SIGNAL(save(QString)), SLOT(save(QString)));

		mainWindow->show();
	}
	catch (JsonParserException& exception)
	{
		QMessageBox::critical(0, "Error", exception.what());
	}

}
void GuiModeController::initControllersPanel(QGroupBox*& controlsPanel)
{
	controlsPanel = new QGroupBox("Controls");

    int x = config->getPositionX();
    int y = config->getPositionY();
    int scale = config->getScale();


	QVBoxLayout* controlsLayout = new QVBoxLayout;
    xControl = new ControlPanel("X", XY_MIN, XY_MAX, x);
    yControl = new ControlPanel("Y", XY_MIN, XY_MAX, y);
    scaleControl = new ControlPanel("Scale", SCALE_MIN, SCALE_MAX, scale);
    fillCheckBox = new QCheckBox("Fill", controlsPanel);
    fillCheckBox->setCheckState(config->isFill() ? Qt::Checked : Qt::Unchecked);
    outlineCheckBox = new QCheckBox("Outline", controlsPanel);
    outlineCheckBox->setCheckState(config->isOutline() ? Qt::Checked : Qt::Unchecked);

    controlsLayout->addWidget(xControl);
    controlsLayout->addWidget(yControl);
    controlsLayout->addWidget(scaleControl);
    controlsLayout->addWidget(fillCheckBox);
    controlsLayout->addWidget(outlineCheckBox);
	
	controlsPanel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
	controlsPanel->setLayout(controlsLayout);

    connect(xControl, SIGNAL(valueChanged(int)), SLOT(setXValue(int)));
    connect(yControl, SIGNAL(valueChanged(int)), SLOT(setYValue(int)));
    connect(scaleControl, SIGNAL(valueChanged(int)), SLOT(setScaleValue(int)));
    connect(fillCheckBox, SIGNAL(stateChanged(int)), SLOT(setFillValue(int)));
    connect(outlineCheckBox, SIGNAL(stateChanged(int)), SLOT(setOutlineValue(int)));
}

void GuiModeController::open(QString str)
{
	if (!str.isEmpty())
	{
		
		try
		{
			Configuration* oldConfig = config;
			config = parser->parse(str);
			delete oldConfig;
			drawPanel->setConfig(config);
			drawPanel->update();

            xControl->setValue(config->getPositionX());
            yControl->setValue(config->getPositionY());
            scaleControl->setValue(config->getScale());
            config->isFill() ? fillCheckBox->setCheckState(Qt::Checked)
                             : fillCheckBox->setCheckState(Qt::Unchecked);
            config->isOutline() ? outlineCheckBox->setCheckState(Qt::Checked)
                                : outlineCheckBox->setCheckState(Qt::Unchecked);
		}
		catch (JsonParserException& exception)
		{
			QMessageBox::critical(0, "Error", exception.what());
		}

	}
}

void GuiModeController::save(QString str) const
{
	if (!str.isEmpty())
	{
		try
		{
			serializer->serialize(str, config);
		}
		catch (saveAndLoadConfig::ConfigSerializerException& exception)
		{
			QMessageBox::critical(0, "Error", exception.what());
		}

	}
}

void GuiModeController::setXValue(int x)
{
    config->setPositionX(x);
    xControl->setValue(config->getPositionX());
	drawPanel->update();
}

void GuiModeController::setYValue(int y)
{
    config->setPositionY(y);
    yControl->setValue(config->getPositionY());
	drawPanel->update();
}

void GuiModeController::setScaleValue(int x)
{
    config->setScale(x);
    drawPanel->update();
}

void GuiModeController::setFillValue(int state)
{
    config->setFill(state == Qt::Checked);
    drawPanel->update();
}

void GuiModeController::setOutlineValue(int state)
{
    config->setOutline(state == Qt::Checked);
    drawPanel->update();
}






