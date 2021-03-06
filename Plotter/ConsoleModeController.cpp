#include "ConsoleModeController.h"
#include "Drawer.h"
#include "ConfigParser.h"
#include <iostream>
#include <QImage>
#include "JsonParserException.h"


ConsoleModeController::ConsoleModeController()
{
}

ConsoleModeController::~ConsoleModeController()
{
}

void ConsoleModeController::run(QString pathToConfigFile, QString pathToOutputFile)
{
	saveAndLoadConfig::ConfigParser parser;
	try
	{
        Configuration* config = parser.parse(pathToConfigFile);
		Drawer drawer;
        Lemniscate* lemniscate = config->getLemniscate();
        int width = config->getPanel()->getWidth();
        int height = config->getPanel()->getHeight();
        QImage* image = new QImage(width, height, QImage::Format_RGB888);

		const int whiteColorValue = 255;
		memset(image->bits(), whiteColorValue, image->byteCount());
        drawer.drawLemniscate(image, lemniscate);


        if (!image->save(pathToOutputFile))
		{
			std::cerr << "image wasn't saved" << std::endl;
		}
	}
	catch (JsonParserException& exception)
	{
		std::cerr << exception.what() << std::endl;
	}
}

