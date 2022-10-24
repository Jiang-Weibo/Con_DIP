#ifndef IMAGESETTINGS_H
#define IMAGESETTINGS_H

#include <QWidget>
#include <QTextEdit>
#include <QTextLine>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtDebug>
#include <QLabel>
#include <QStringList>
class ImageSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ImageSettings(QWidget *parent = nullptr);
    QLineEdit *getLinearInut1() const;
    void setLinearInut1(QLineEdit *value);

    QLineEdit *getLinearInut2() const;
    void setLinearInut2(QLineEdit *value);

    QLineEdit *getLinearInut3() const;
    void setLinearInut3(QLineEdit *value);

    QLineEdit *getLinearInut4() const;
    void setLinearInut4(QLineEdit *value);

    QLineEdit *getLinearInut5() const;
    void setLinearInut5(QLineEdit *value);

    QLineEdit *getLinearInut6() const;
    void setLinearInut6(QLineEdit *value);

    QLineEdit *getLogInput() const;
    void setLogInput(QLineEdit *value);

    QLineEdit *getMeanFilterRow() const;
    void setMeanFilterRow(QLineEdit *value);

    QLineEdit *getMeanFilterCol() const;
    void setMeanFilterCol(QLineEdit *value);

    QLineEdit *getMedianFilterRow() const;
    void setMedianFilterRow(QLineEdit *value);

    QLineEdit *getMedianFilterCol() const;
    void setMedianFilterCol(QLineEdit *value);

    QLineEdit *getGaussNoiseMu() const;
    void setGaussNoiseMu(QLineEdit *value);

    QLineEdit *getGaussNoiseSigma() const;
    void setGaussNoiseSigma(QLineEdit *value);

    QLineEdit *getGaussNoiseK() const;
    void setGaussNoiseK(QLineEdit *value);

    QLineEdit *getImpulseNoise() const;
    void setImpulseNoise(QLineEdit *value);

    QLineEdit *getOriginalWeight() const;
    void setOriginalWeight(QLineEdit *value);

    QLineEdit *getProcessedWeight() const;
    void setProcessedWeight(QLineEdit *value);

    QLineEdit *getRotateFactor() const;
    void setRotateFactor(QLineEdit *value);

    QLineEdit *getScaleFactor() const;
    void setScaleFactor(QLineEdit *value);

private:
    QLineEdit* linearInut1;
    QLineEdit* linearInut2;
    QLineEdit* linearInut3;
    QLineEdit* linearInut4;
    QLineEdit* linearInut5;
    QLineEdit* linearInut6;
    QLineEdit* logInput;
    QLineEdit* meanFilterRow;
    QLineEdit* meanFilterCol;
    QLineEdit* medianFilterRow;
    QLineEdit* medianFilterCol;
    QLineEdit* gaussNoiseMu;
    QLineEdit* gaussNoiseSigma;
    QLineEdit* gaussNoiseK;
    QLineEdit* impulseNoise;
    QLineEdit* originalWeight;
    QLineEdit* processedWeight;
    QLineEdit* rotateFactor;
    QLineEdit* scaleFactor;
    //int count = 0;
protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
};

#endif // IMAGESETTINGS_H
