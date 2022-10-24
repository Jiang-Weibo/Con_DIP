#include "imagesettings.h"

ImageSettings::ImageSettings(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Settings");
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();

    QVBoxLayout* vBoxLayout0 = new QVBoxLayout; // left
    QVBoxLayout* vBoxLayout1 = new QVBoxLayout;// middle
    QVBoxLayout* vBoxLayout2 = new QVBoxLayout;//right
    QHBoxLayout* hBoxLayout = new QHBoxLayout;

    linearInut1 = new QLineEdit(this);
    linearInut2 = new QLineEdit(this);
    linearInut3 = new QLineEdit(this);
    linearInut4 = new QLineEdit(this);
    linearInut5 = new QLineEdit(this);
    linearInut6 = new QLineEdit(this);
    meanFilterRow = new QLineEdit("3",this);
    meanFilterCol = new QLineEdit("3",this);
    medianFilterRow = new QLineEdit("3",this);
    medianFilterCol = new QLineEdit("3",this);
    logInput = new QLineEdit("50.0,0.8,1.05",this);
    gaussNoiseMu = new QLineEdit(this);
    gaussNoiseSigma = new QLineEdit(this);
    gaussNoiseK = new QLineEdit(this);
    impulseNoise = new QLineEdit(this);
    originalWeight = new QLineEdit("0.5",this);
    processedWeight = new QLineEdit("0.5",this);
    rotateFactor = new QLineEdit("90",this);
    scaleFactor = new QLineEdit("1.0",this);
    QLineEdit*tempNull = new QLineEdit("NotAccessible",this);
    QLineEdit*tempNull1 = new QLineEdit("NotAccessible",this);
    QLineEdit*tempNull2 = new QLineEdit("NotAccessible",this);
    QLineEdit*tempNull3 = new QLineEdit("NotAccessible",this);
    QLineEdit*tempNull4 = new QLineEdit("NotAccessible",this);
    QLineEdit*tempNull5 = new QLineEdit("NotAccessible",this);
    QLineEdit*tempNull6 = new QLineEdit("NotAccessible",this);
    tempNull->setEnabled(false);
    tempNull1->setEnabled(false);
    tempNull2->setEnabled(false);
    tempNull3->setEnabled(false);
    tempNull4->setEnabled(false);
    tempNull5->setEnabled(false);
    tempNull6->setEnabled(false);
    QLabel* linear1 = new QLabel("LinearTrans.1");
    QLabel* linear2 = new QLabel("LinearTrans.2");
    QLabel* linear3 = new QLabel("LinearTrans.3");
    QLabel* logarithm = new QLabel("log a,b,c");
    QLabel* meanFilter = new QLabel("MeanFilter");
    QLabel* medianFilter = new QLabel("MedianFilter");
    QLabel* gaussMu  = new QLabel("GaussNoiseMu");
    QLabel* gaussSigma  = new QLabel("gaussSigma");
    QLabel* gaussK  = new QLabel("gaussK");
    QLabel* impulseSNR = new QLabel("NSR");
    QLabel* addWeight = new QLabel("originalWeight, processWeight");
    QLabel* rotateFactorLabel = new QLabel("rotate");
    QLabel* scaleFactorLabel = new QLabel("scale");
    //QLabel* original = new QLabel("originalWeight");

    vBoxLayout0->addWidget(linear1);
    vBoxLayout0->addWidget(linear2);
    vBoxLayout0->addWidget(linear3);
    vBoxLayout0->addWidget(logarithm);
    vBoxLayout0->addWidget(meanFilter);
    vBoxLayout0->addWidget(medianFilter);
    vBoxLayout0->addWidget(gaussMu);
    vBoxLayout0->addWidget(gaussSigma);
    vBoxLayout0->addWidget(gaussK);
    vBoxLayout0->addWidget(impulseSNR);
    vBoxLayout0->addWidget(addWeight);
    vBoxLayout0->addWidget(rotateFactorLabel);
    vBoxLayout0->addWidget(scaleFactorLabel);

    vBoxLayout1->addWidget(linearInut1);
    vBoxLayout1->addWidget(linearInut2);
    vBoxLayout1->addWidget(linearInut3);
    vBoxLayout1->addWidget(logInput);
    vBoxLayout1->addWidget(meanFilterRow);
    vBoxLayout1->addWidget(medianFilterRow);
    vBoxLayout1->addWidget(gaussNoiseMu);
    vBoxLayout1->addWidget(gaussNoiseSigma);
    vBoxLayout1->addWidget(gaussNoiseK);
    vBoxLayout1->addWidget(impulseNoise);
    vBoxLayout1->addWidget(originalWeight);
    vBoxLayout1->addWidget(rotateFactor);
    vBoxLayout1->addWidget(scaleFactor);

    //vBoxLayout1->addWidget()

    vBoxLayout2->addWidget(linearInut4);
    vBoxLayout2->addWidget(linearInut5);
    vBoxLayout2->addWidget(linearInut6);
    vBoxLayout2->addWidget(tempNull);
    vBoxLayout2->addWidget(meanFilterCol);
    vBoxLayout2->addWidget(medianFilterCol);
    vBoxLayout2->addWidget(tempNull1);
    vBoxLayout2->addWidget(tempNull2);
    vBoxLayout2->addWidget(tempNull3);
    vBoxLayout2->addWidget(tempNull4);
    vBoxLayout2->addWidget(processedWeight);
    vBoxLayout2->addWidget(tempNull5);
    vBoxLayout2->addWidget(tempNull6);


    hBoxLayout->addLayout(vBoxLayout0);
    hBoxLayout->addLayout(vBoxLayout1);
    hBoxLayout->addLayout(vBoxLayout2);

    this->setLayout(hBoxLayout);
    QString temp = linearInut1->text();
    QStringList tempList= temp.split(",");
}

QLineEdit *ImageSettings::getLinearInut1() const
{
    return linearInut1;
}

void ImageSettings::setLinearInut1(QLineEdit *value)
{
    linearInut1 = value;
}

QLineEdit *ImageSettings::getLinearInut2() const
{
    return linearInut2;
}

void ImageSettings::setLinearInut2(QLineEdit *value)
{
    linearInut2 = value;
}

QLineEdit *ImageSettings::getLinearInut3() const
{
    return linearInut3;
}

void ImageSettings::setLinearInut3(QLineEdit *value)
{
    linearInut3 = value;
}

QLineEdit *ImageSettings::getLinearInut4() const
{
    return linearInut4;
}

void ImageSettings::setLinearInut4(QLineEdit *value)
{
    linearInut4 = value;
}

QLineEdit *ImageSettings::getLinearInut5() const
{
    return linearInut5;
}

void ImageSettings::setLinearInut5(QLineEdit *value)
{
    linearInut5 = value;
}

QLineEdit *ImageSettings::getLinearInut6() const
{
    return linearInut6;
}

void ImageSettings::setLinearInut6(QLineEdit *value)
{
    linearInut6 = value;
}

QLineEdit *ImageSettings::getLogInput() const
{
    return logInput;
}

void ImageSettings::setLogInput(QLineEdit *value)
{
    logInput = value;
}

QLineEdit *ImageSettings::getMeanFilterRow() const
{
    return meanFilterRow;
}

void ImageSettings::setMeanFilterRow(QLineEdit *value)
{
    meanFilterRow = value;
}

QLineEdit *ImageSettings::getMeanFilterCol() const
{
    return meanFilterCol;
}

void ImageSettings::setMeanFilterCol(QLineEdit *value)
{
    meanFilterCol = value;
}

QLineEdit *ImageSettings::getMedianFilterRow() const
{
    return medianFilterRow;
}

void ImageSettings::setMedianFilterRow(QLineEdit *value)
{
    medianFilterRow = value;
}

QLineEdit *ImageSettings::getMedianFilterCol() const
{
    return medianFilterCol;
}

void ImageSettings::setMedianFilterCol(QLineEdit *value)
{
    medianFilterCol = value;
}

QLineEdit *ImageSettings::getGaussNoiseMu() const
{
    return gaussNoiseMu;
}

void ImageSettings::setGaussNoiseMu(QLineEdit *value)
{
    gaussNoiseMu = value;
}

QLineEdit *ImageSettings::getGaussNoiseSigma() const
{
    return gaussNoiseSigma;
}

void ImageSettings::setGaussNoiseSigma(QLineEdit *value)
{
    gaussNoiseSigma = value;
}

QLineEdit *ImageSettings::getGaussNoiseK() const
{
    return gaussNoiseK;
}

void ImageSettings::setGaussNoiseK(QLineEdit *value)
{
    gaussNoiseK = value;
}

QLineEdit *ImageSettings::getImpulseNoise() const
{
    return impulseNoise;
}

void ImageSettings::setImpulseNoise(QLineEdit *value)
{
    impulseNoise = value;
}

QLineEdit *ImageSettings::getOriginalWeight() const
{
    return originalWeight;
}

void ImageSettings::setOriginalWeight(QLineEdit *value)
{
    originalWeight = value;
}

QLineEdit *ImageSettings::getProcessedWeight() const
{
    return processedWeight;
}

void ImageSettings::setProcessedWeight(QLineEdit *value)
{
    processedWeight = value;
}

QLineEdit *ImageSettings::getRotateFactor() const
{
    return rotateFactor;
}

void ImageSettings::setRotateFactor(QLineEdit *value)
{
    rotateFactor = value;
}

QLineEdit *ImageSettings::getScaleFactor() const
{
    return scaleFactor;
}

void ImageSettings::setScaleFactor(QLineEdit *value)
{
    scaleFactor = value;
}

void ImageSettings::paintEvent(QPaintEvent *event)
{

}
