#include "widget.h"
using namespace cv;
using namespace std; 
int cmp(const void *p1, const void *p2)
{
    return *(int*)p1-*(int*)p2;
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("widget");
    this->setMinimumSize(600,600);
    m_beginMouse = QPoint(0,0);
    m_endMouse = QPoint(0,0);
    originalLabel = new QLabel("originalLabel");
    processedLabel = new QLabel("processedLabel");

    originalLabel->setPixmap(QPixmap("F:\\JNU\\DigitalImageProcess\\DigitalImageProcessWork\\bmp\\lena.bmp"));
    processedLabel->setPixmap(QPixmap("F:\\JNU\\DigitalImageProcess\\DigitalImageProcessWork\\bmp\\Image.bmp"));
    processedLabel->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    setter = new ImageSettings(this);


    menuBar = new QMenuBar(this);
    menuBar->setStyleSheet("QMenuBar:item{background-color:#999999;}QMenuBar{background-color:#629222;}");
    fileMenu = new QMenu("File");
    transformMenu = new QMenu("transform");
    enhancementMenu = new QMenu("enhance");
    encodeAndDivisionMenu = new QMenu("codeAndDiv");
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(transformMenu);
    menuBar->addMenu(enhancementMenu);
    menuBar->addMenu(encodeAndDivisionMenu);
    //fileMenu
    QAction* openFile = fileMenu->addAction("openfile");
    connect(openFile,&QAction::triggered,this,[&](){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            return;
        }
        img = imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE);
        imshow("Picture",img);
        qDebug()<<"img.type()="<<img.type();
        int nr = getClosestPower(img.rows);
        int nc = getClosestPower(img.cols);
        this->tdComplex = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(getClosestPower(nc)))*(static_cast<int>(getClosestPower(nr)))];
        this->fdComplex = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(getClosestPower(nc)))*(static_cast<int>(getClosestPower(nr)))];

    });

    //transformMenu
    QAction* fourier = transformMenu->addAction("fourier");
    connect(fourier,&QAction::triggered,this,[&](){
        img.convertTo(tempMat,CV_64FC1);
        int nr = getClosestPower(tempMat.rows);
        int nc = getClosestPower(tempMat.cols);
        MyComplex<double>*tdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        MyComplex<double>*fdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        double* tempRow;
        //assignment
        for (int i=0;i<tempMat.rows;i++) {
            tempRow = tempMat.ptr<double>(i);
            for(int j=0;j<tempMat.cols;j++) {
                tdComplexTemp[i*nc+j].rel=tempRow[j];
                tdComplexTemp[i*nc+j].img=0.0;
            }
        }

        Widget::Fourier(nr,nc,tdComplexTemp,fdComplexTemp);

        showMat = Mat(nr,nc,CV_64FC1);
        for(int i=0;i<nr;i++){
            double *tempRow = showMat.ptr<double>(i);
            for(int j=0;j<nc;j++){
                double ans = fdComplexTemp[i*nc+j].img*fdComplexTemp[i*nc+j].img+fdComplexTemp[i*nc+j].rel*fdComplexTemp[i*nc+j].rel;
                tempRow[j]=sqrt(ans)/100.0;
            }
        }

        showMat.convertTo(showMat,CV_8UC1);
        imshow("afterFFT",showMat);
    });
    QAction* inverseFourier = transformMenu->addAction("inverFourier");
    connect(inverseFourier,&QAction::triggered,[&](){
        int nr = getClosestPower(tempMat.rows);
        int nc = getClosestPower(tempMat.cols);
        MyComplex<double>*tdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        MyComplex<double>*fdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        memcpy(fdComplexTemp,fdComplex,sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr)));
        Widget::InverseFourier(nr,nc,tdComplexTemp,fdComplexTemp);

        showMat = Mat(nr,nc,CV_64FC1);

        for(int i=0;i<nr;i++){
            double *tempRow = showMat.ptr<double>(i);
            for(int j=0;j<nc;j++){
                tempRow[j]=tdComplexTemp[i*nc+j].rel;
            }
        }
        showMat.convertTo(showMat,CV_8UC1);
        imshow("afterIFFT",showMat);
    });
    QAction* disCos = transformMenu->addAction("discreteCosine");
    connect(disCos,&QAction::triggered,[&](){
        img.convertTo(tempMat,CV_64FC1);
        int nr = getClosestPower(tempMat.rows);
        int nc = getClosestPower(tempMat.cols);
        MyComplex<double>*tdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        MyComplex<double>*fdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        double* tempRow;
        //assignment
        for (int i=0;i<tempMat.rows;i++) {
            tempRow = tempMat.ptr<double>(i);
            for(int j=0;j<tempMat.cols;j++){
                tdComplexTemp[i*nc+j].rel=tempRow[j];
                tdComplexTemp[i*nc+j].img=0.0;
            }
        }
        Widget::DisCosine(nr,nc,tdComplexTemp,fdComplexTemp);

        showMat = Mat(nr,nc,CV_64FC1);
        for(int i=0;i<nr;i++){
            double *tempRow = showMat.ptr<double>(i);
            for(int j=0;j<nc;j++){
                double ans = fabs(fdComplexTemp[j+i*nc].rel);
                if(ans>255) ans=255;
                tempRow[j]=ans;
            }
        }
        showMat.convertTo(showMat,CV_8UC1);
        imshow("afterDCT",showMat);
    });
    QAction* inverseCos = transformMenu->addAction("inverseCosine");
    connect(inverseCos,&QAction::triggered,[&](){
        int nr = getClosestPower(img.rows);
        int nc = getClosestPower(img.cols);
        MyComplex<double>*tdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        MyComplex<double>*fdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        memcpy(fdComplexTemp,fdComplex,sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr)));
        Widget::InverseDisCosine(nr,nc,tdComplexTemp,fdComplexTemp);

        showMat = Mat(nr,nc,CV_64FC1);
        for(int i=0;i<nr;i++){
            double *tempRow = showMat.ptr<double>(i);
            for(int j=0;j<nc;j++){
                tempRow[j]=tdComplexTemp[i*nc+j].rel;
            }
        }
        showMat.convertTo(showMat,CV_8UC1);
        imshow("afterIDCT",showMat);
    });
    QAction* walsh = transformMenu->addAction("walsh");
    connect(walsh,&QAction::triggered,[&](){
        img.convertTo(tempMat,CV_64FC1);
        int nr = getClosestPower(img.rows);
        int nc = getClosestPower(img.cols);
        MyComplex<double>*tdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        MyComplex<double>*fdComplexTemp = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nc))*(static_cast<int>(nr))];
        double* tempRow;
        //assignment
        for (int i=0;i<tempMat.rows;i++) {
            tempRow = tempMat.ptr<double>(i);
            for(int j=0;j<tempMat.cols;j++){
                tdComplexTemp[i*nc+j].rel=tempRow[j];
                fdComplexTemp[i*nc+j].rel=fdComplexTemp[i*nc+j].img=tdComplexTemp[i*nc+j].img=0.0;
            }
        }
        Widget::Walsh(nr,nc,tdComplexTemp,fdComplexTemp);

        showMat = Mat(nr,nc,CV_64FC1);
        for(int i=0;i<nr;i++){
            double *tempRow = showMat.ptr<double>(i);
            for(int j=0;j<nc;j++){
                double ans = fabs(fdComplexTemp[j*nr+i].rel*1000);
                tempRow[j]=ans;
            }
        }
        showMat.convertTo(showMat,CV_8UC1);
        imshow("afterWalsh",showMat);
    });


    //enhancementMenu
    QAction* linear = enhancementMenu->addAction("linearTrans");
    connect(linear,&QAction::triggered,[&](){
        Mat showMat;
        img.copyTo(showMat);
        if(showMat.empty()) return;
        //Input
        QStringList list1 = setter->getLinearInut1()->text().split(",");
        QStringList list2 = setter->getLinearInut2()->text().split(",");
        QStringList list3 = setter->getLinearInut3()->text().split(",");
        //Output
        QStringList list4 = setter->getLinearInut4()->text().split(",");
        QStringList list5 = setter->getLinearInut5()->text().split(",");
        QStringList list6 = setter->getLinearInut6()->text().split(",");
        //qDebug()<<"list1[0] = "<<list1[0].toInt()<<" list1[1] = "<<list1[1].toInt();
        //qDebug()<<"list4[0] = "<<list4[0].toInt()<<" list4[1] = "<<list4[1].toInt();
        for(int i=0;i<showMat.rows;i++){
            uchar* tempRow = showMat.ptr<uchar>(i);
            for(int j=0;j<showMat.cols;j++){
                uchar t = tempRow[j];
                if(!list1[0].isEmpty()&&!list1[1].isEmpty()&&t>=list1[0].toInt()&&t<=list1[1].toInt()) {
                    t = (uchar)linearTrans(list1[0].toInt(),list1[1].toInt(),list4[0].toInt(),list4[1].toInt(),(int)t);
                    tempRow[j] = t;
                }
                else if(!list2[0].isEmpty()&&!list2[1].isEmpty()&&t>=list2[0].toInt()&&t<=list2[1].toInt()) {
                    t = (uchar)linearTrans(list2[0].toInt(),list2[1].toInt(),list5[0].toInt(),list5[1].toInt(),(int)t);
                    tempRow[j] = t;
                    //qDebug()<<"2 called";
                }
                else if(!list3[0].isEmpty()&&!list3[1].isEmpty()&&t>=list3[0].toInt()&&t<=list3[1].toInt()) {
                    t = (uchar)linearTrans(list3[0].toInt(),list3[1].toInt(),list6[0].toInt(),list6[1].toInt(),(int)t);
                    tempRow[j] = t;
                    //qDebug()<<"3 called";
                }
            }
        }
        imshow("afterLinearMap",showMat);
    });
    QAction* log = enhancementMenu->addAction("logTrans");
    connect(log,&QAction::triggered,[&](){
        Mat showMat;
        img.copyTo(showMat);
        if(showMat.empty()) return;
        QStringList l = setter->getLogInput()->text().split(",");
        float a=50.0;
        float b=0.8;
        float c=1.05;
        if(!l[0].isNull()&&!l[0].isEmpty()) a = l[0].toFloat();
        if(!l[1].isNull()&&!l[1].isEmpty()) b = l[1].toFloat();
        if(!l[2].isNull()&&!l[2].isEmpty()) c = l[2].toFloat();
        //qDebug()<<a<<""<<b<<""<<c;
        for(int i=0;i<showMat.rows;i++){
            uchar* tempRow = showMat.ptr<uchar>(i);
            for(int j=0;j<showMat.cols;j++){
                uchar t = tempRow[j];
                tempRow[j] = (uchar)logTrans(a,b,c,t);
                //qDebug()<<tempRow[j];
            }
        }
        imshow("afterLogMap",showMat);
    });
    QAction* equalization = enhancementMenu->addAction("equalization");
    connect(equalization,&QAction::triggered,[&](){
        int gray[256] = { 0 };  //记录每个灰度级别下的像素个数
        double gray_prob[256] = { 0 };  //记录灰度分布密度
        double gray_distribution[256] = { 0 };  //记录累计密度
        int gray_equal[256] = { 0 };  //均衡化后的灰度值

        int gray_sum = 0;  //像素总数
        Mat showMat;
        img.copyTo(showMat);
        if(showMat.empty()) return;
        //Mat output = input.clone();
        gray_sum = showMat.cols * showMat.rows;

        //统计每个灰度下的像素个数
        for (int i = 0; i < showMat.rows; i++)
        {
            uchar* p = showMat.ptr<uchar>(i);
            for (int j = 0; j < showMat.cols; j++)
            {
                int vaule = p[j];
                gray[vaule]++;
            }
        }


        //统计灰度频率
        for (int i = 0; i < 256; i++)
        {
            gray_prob[i] = ((double)gray[i] / gray_sum);
        }

        //计算累计密度
        gray_distribution[0] = gray_prob[0];
        for (int i = 1; i < 256; i++)
        {
            gray_distribution[i] = gray_distribution[i-1] +gray_prob[i];
        }

        //重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
        for (int i = 0; i < 256; i++)
        {
            gray_equal[i] = (uchar)(255 * gray_distribution[i] + 0.5);
        }


        //直方图均衡化,更新原图每个点的像素值
        for (int i = 0; i < showMat.rows; i++)
        {
            uchar* p = showMat.ptr<uchar>(i);
            for (int j = 0; j < showMat.cols; j++)
            {
                p[j] = gray_equal[p[j]];
            }
        }

        imshow("afterEqualization",showMat);
    });
    QAction* meanFilter = enhancementMenu->addAction("meanFilter");
    connect(meanFilter,&QAction::triggered,[&](){
        int row = setter->getMeanFilterRow()->text().toInt();
        int col = setter->getMeanFilterCol()->text().toInt();
        if(row<=0 || col<=0){return;}
        uchar* arr = new uchar[row*col];
        cv::Mat showMat;
        img.copyTo(showMat);
        if(showMat.empty()) return;
        for(int i=0;i<showMat.rows-row;i++){
            for(int j=0;j<showMat.cols-col;j++){
                for(int m=0;m<row;m++){
                    uchar* tempRow = showMat.ptr<uchar>(i+m);
                    for(int n=0;n<col;n++){
                        arr[n+m*col] = tempRow[j+n];
                    }
                }
                float ans = 0;
                for(int a = 0;a < row*col;a++){
                    ans+=arr[a];
                }
                ans/=(row*col);
                showMat.ptr<uchar>(i+row/2)[j+col/2] = (uchar)(int)ans;
            }
        }
        imshow("afterMeanFilter",showMat);
    });
    QAction* medianFilter = enhancementMenu->addAction("medianFilter");
    connect(medianFilter,&QAction::triggered,[&](){
        int row = setter->getMedianFilterRow()->text().toInt();
        int col = setter->getMedianFilterCol()->text().toInt();
        if(row<=0 || col<=0) return;
        Mat showMat;
        img.copyTo(showMat);
        if(showMat.empty()) return;
        uchar* rowArr = new uchar[row];
        uchar* colArr = new uchar[col];
        for(int i=0;i<showMat.rows;i++){
            uchar* tempRow = showMat.ptr<uchar>(i);
            for(int j=0;j<showMat.cols;j++){
                for(int k=0;k<row;k++){
                    rowArr[k] = tempRow[k+j];
                }
                qsort(rowArr,row,sizeof(uchar),cmp);
                tempRow[j+row/2] = rowArr[row/2];
            }
        }
        cv::transpose(showMat,showMat);
        for(int i=0;i<showMat.rows;i++){
            uchar* tempRow = showMat.ptr<uchar>(i);
            for(int j=0;j<showMat.cols;j++){
                for(int k=0;k<col;k++){
                    colArr[k] = tempRow[k+j];
                }
                qsort(colArr,col,sizeof(uchar),cmp);
                tempRow[j+col/2] = colArr[col/2];
            }
        }
        cv::transpose(showMat,showMat);
        imshow("afterMedianFilter",showMat);
    });
    QAction* capture  = enhancementMenu->addAction("capture");
    connect(capture,&QAction::triggered,[&](){
        isCapture = true;
    });
    QAction* addPixmap = enhancementMenu->addAction("addPixmap");
    connect(addPixmap,&QAction::triggered,[&](){
        //isMove = true;
        img = CVS::QPixmapToCvMat(*originalLabel->pixmap());
        tempMat = CVS::QPixmapToCvMat(*processedLabel->pixmap());
        if(tempMat.rows>img.rows || tempMat.cols>img.cols) {
            QMessageBox::information(this,"warning","captures image is bigger than img");
            return;
        }

        double processWeight = setter->getProcessedWeight()->text().toDouble();
        double originalWeight = setter->getOriginalWeight()->text().toDouble();
        if(processWeight+originalWeight>1.0){
            QMessageBox::information(this,"warning","weightError");
            return;
        }
        int cols = std::min(img.cols,tempMat.cols)*img.channels();
        int rows = std::min(img.rows,tempMat.rows);

        Mat finalMat;
        if(rows==img.rows){
            tempMat.copyTo(finalMat);
        }else{
            img.copyTo(finalMat);
        }

        //imshow("beforeAdd",finalMat);
        for(int i=0;i<rows;i++){
            uchar* tempRow = tempMat.ptr<uchar>(i);
            uchar* imgRow = img.ptr<uchar>(i);
            uchar* finalRow = finalMat.ptr<uchar>(i);
            for(int j=0;j<cols;j++){
                finalRow[j] = (int)(tempRow[j]*processWeight+imgRow[j]*originalWeight);
            }
        }
        finalMat.copyTo(img);
        cv::imshow("afterAdd",finalMat);
    });
    QAction* subPixmap = enhancementMenu->addAction("subPixmap");
    connect(subPixmap,&QAction::triggered,[&](){
        double processWeight = setter->getProcessedWeight()->text().toDouble();
        double originalWeight = setter->getOriginalWeight()->text().toDouble();
        if(processWeight+originalWeight>1.0){
            QMessageBox::information(this,"warning","weightError");
            return;
        }
        int cols = std::min(img.cols,tempMat.cols)*img.channels();
        int rows = std::min(img.rows,tempMat.rows);
        Mat finalMat;
        if(rows==img.rows){
            tempMat.copyTo(finalMat);
        }else{
            img.copyTo(finalMat);
        }
        for(int i=0;i<rows;i++){
            uchar* tempRow = tempMat.ptr<uchar>(i);
            uchar* imgRow = img.ptr<uchar>(i);
            uchar* finalRow = finalMat.ptr<uchar>(i);
            for(int j=0;j<cols;j++){
                finalRow[j] = finalRow[j]-tempRow[j]*processWeight;
                //finalRow[j] = (int)(tempRow[j]*processWeight+imgRow[j]*originalWeight);
            }
        }
        //finalMat.copyTo(img);
        cv::imshow("afterSub",finalMat);
    });
    QAction* gaussNoise = enhancementMenu->addAction("gaussNoise");
    connect(gaussNoise,&QAction::triggered,[&](){
        if(img.empty()){
            QMessageBox::information(this,"info","You didn't open any image!");
            return;
        }

        Mat showMat = AddNoise(img,setter->getGaussNoiseMu()->text().toDouble(),setter->getGaussNoiseSigma()->text().toDouble(),setter->getGaussNoiseK()->text().toInt());
        afterPixmap = CVS::cvMatToQPixmap(showMat);
        imshow("gaussNoise",showMat);
    });
    QAction* impulsNoise = enhancementMenu->addAction("impulsNoise");
    connect(impulsNoise,&QAction::triggered,[&](){
        if(img.empty()){
            QMessageBox::information(this,"info","You didn't open any image!");
            return;
        }

        Mat showMat = AddNoise(img,setter->getImpulseNoise()->text().toDouble());
        afterPixmap = CVS::cvMatToQPixmap(showMat);
        imshow("impulseNoise",showMat);
    });
    QAction* transpose = enhancementMenu->addAction("transpose");
    connect(transpose,&QAction::triggered,[&](){
        double angle = setter->getRotateFactor()->text().toDouble()*PI/180.0f;
        double matrix[3][3] = {{cos(angle),sin(angle),0.0},{-sin(angle),cos(angle),0.0},{0.0,0.0,1.0}};
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                qDebug()<<matrix[i][j];
        if(img.empty()){
            QMessageBox::information(this,"info","You didn't open any image!");
            return;
        }
        //int edge = (int)sqrt(img.rows*img.rows+img.cols*img.cols);
        Mat showMat = Mat(img.rows*2,img.cols*2,img.type());
        int col = img.cols*img.channels();
        int row = img.rows;
        for(int i=0;i<row;i++){
            uchar* tempRow = img.ptr<uchar>(i);
            int x,y;
            for(int j=0;j<col;j++){
                x = (int)(i*matrix[0][0]-j*matrix[0][1]);
                y = (int)(i*matrix[0][1]+j*matrix[1][1]);
                //int x = (int)(i*cos(angle)-j*sin(angle));
                //int y = (int)(i*sin(angle)+j*cos(angle));
                //if(x<0) x=0;
                //if(y<0) y=0;
                //qDebug()<<"x y"<<x<<" "<<y;
                if(x<showMat.rows && y<showMat.cols*showMat.channels()){
                    showMat.ptr<uchar>(x+row)[y+col] = tempRow[j];
                }
            }
        }
        imshow("afterTranspose",showMat);
    });
    QAction* scale = enhancementMenu->addAction("scale");
    connect(scale,&QAction::triggered,[&](){
        double scaleFactor = setter->getScaleFactor()->text().toDouble();
        if(img.empty()){
            QMessageBox::information(this,"error","img is empty!");
            return;
        }

        Mat showTemp;
        if(scaleFactor>1.0){
            showTemp = Mat((int)(img.rows*scaleFactor),(int)(img.cols*scaleFactor),img.type());
            int col = showTemp.cols*showTemp.channels();
            int row = showTemp.rows;
            for(int i=0;i<row;i++){
                for(int j=0;j<col;j++){
                    double x0 = i/scaleFactor,y0 = j/scaleFactor;
                    double u = x0-(int)x0,v = y0-(int)y0;
                    int x = (int)x0,y = (int)y0;
                    if(x+1<img.rows && y+1<img.cols)
                        showTemp.ptr<uchar>(i)[j] = (1.-u)*(1.-v)*img.ptr<uchar>(x)[y]+(1.-u)*v*img.ptr<uchar>(x)[y+1]+u*(1.-v)*img.ptr<uchar>(x+1)[y]+u*v*img.ptr<uchar>(x+1)[y+1];
                    else
                        showTemp.ptr<uchar>(i)[j] = 255;
                }
            }
            imshow("afterScale",showTemp);
        }else if(scaleFactor<1.0 && scaleFactor>0.0){
            showTemp = Mat(img.rows,img.cols,img.type());
            int col = img.cols*img.channels();
            int row = img.rows;
            for(int i=0;i<row;i++){
                //uchar* imgRow = img.ptr<uchar>(i);
                for(int j=0;j<col;j++){
                    int x = (int)(i*scaleFactor);
                    int y = (int)(j*scaleFactor);
                    showTemp.at<uchar>(x,y) = img.at<uchar>(i,j);
                }
            }
            imshow("afterScale",showTemp);
        }
    });

    //codeAndDivision
    QAction* HuffmanCode = encodeAndDivisionMenu->addAction("HuffmanCode");
    connect(HuffmanCode,&QAction::triggered,[&](){
        Mat originalMat;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            //QMessageBox::warning(this,"warning","open failed!!");
            return;
        }
        originalMat = imread(fileName.toStdString(),cv::IMREAD_COLOR);
        imshow("orignalImage",originalMat);

        cvBGR2HSI(originalMat);// I is Lightness
        int* arr = new int[256];

        vector<myNode::Node*> myList = statisticFrequency(originalMat,arr);
        //for(int i=0;i<256;i++) qDebug()<<arr[i];
        myNode::Node* root = myNode::Node::buildHuffmanTree(myList);

        std::string begin = "";
        m_map = new QMap<int,string>();
        myNode::Node::inorderTraversal(root, begin, m_map);
        qDebug()<<"inorder is over!";
        QMap<int,string>::iterator its;
        //qDebug()<<"1 length is"<<QString::fromStdString(m_map->find(1).value());

        for(its = m_map->begin();its!=m_map->end();++its) qDebug()<<"key "<<its.key()<<" code"<<QString::fromStdString(its.value());
        if(compressionAndEfficiency(arr))
            imshow("HSIModel",originalMat);
        else
            QMessageBox::information(this,"error","compress is failed.");
    });
    QAction* ShannonFanoCode = encodeAndDivisionMenu->addAction("ShannonFanoCode");
    connect(ShannonFanoCode,&QAction::triggered,[&](){
        Mat originalMat;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            return;
        }
        originalMat = imread(fileName.toStdString(),cv::IMREAD_COLOR);
        imshow("orignalImage",originalMat);

        cvBGR2HSI(originalMat);// I is Lightness

        int* arr = new int[256] () ;

        statisticFrequency(originalMat,arr);
        int count = 0;
        for(int i=0;i<256;i++){
            if(arr[i]!=0){
                count++;
            }
        }
        myNode::Node* ShannonTree = new myNode::Node[count] () ;
        int j=0;
        for(int i=0;i<256;i++){
            if(arr[i]!=0){
                ShannonTree[j].value = i;
                ShannonTree[j].weight = arr[i];
                j++;
            }
        }
        for(int i=0;i<256;i++){
            for (int j=0;j<255;j++) {
                myNode::Node temp;
                if(ShannonTree[j].weight<ShannonTree[j+1].weight){
                    temp = ShannonTree[j];
                    ShannonTree[j] = ShannonTree[j+1];
                    ShannonTree[j+1] = temp;
                }
            }
        }
        string begin = "";
        myNode::Node::buildShannonFanoTree(ShannonTree,0,count-1,begin);
        for (int i=0;i<256;i++) {
         qDebug()<<i<<" code "<<QString::fromStdString(ShannonTree[i].code);
        }
    });
    QAction* sobelOperator = encodeAndDivisionMenu->addAction("sobelOperator");
    connect(sobelOperator,&QAction::triggered,[&](){
        Mat originalMat;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            return;
        }
        int threshold = QInputDialog::getInt(this,"comentropy","Plz input threadhold",5,0,255);
        originalMat = imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE);
        float comentropy1 = computComentropy(originalMat);
        imshow("beforeSobel",originalMat);

        Mat showMat;
        originalMat.copyTo(showMat);
        for (int i=1;i<originalMat.rows-1;i++) {
            for(int j=1;j<originalMat.cols-1;j++){
                int x = (originalMat.at<uchar>(i-1,j+1)+2*originalMat.at<uchar>(i,j+1)+originalMat.at<uchar>(i+1,j+1))-(originalMat.at<uchar>(i-1,j-1)+2*originalMat.at<uchar>(i,j-1)+originalMat.at<uchar>(i+1,j-1));
                int y = (originalMat.at<uchar>(i-1,j-1)+2*originalMat.at<uchar>(i-1,j)+originalMat.at<uchar>(i-1,j+1))-(originalMat.at<uchar>(i+1,j-1)+2*originalMat.at<uchar>(i+1,j)+originalMat.at<uchar>(i+1,j+1));
                float t = sqrt(x*x+y*y)+0.5f;
                if(t>255.0f)
                    t = 255.0f;
                showMat.at<uchar>(i,j) = t<=threshold ? 0:255;
            }
        }
        Mat out = showMat.clone();
        for(int i=0;i<originalMat.rows;i++){
            for(int j=0;j<originalMat.cols;j++){
                out.at<uchar>(i,j) = showMat.at<uchar>(i,j)==0?0:originalMat.at<uchar>(i,j);
            }
        }
        imshow("afterSobel",showMat);
        //addWeighted(showMat,0.5,originalMat,0.5,0,out);
        imshow("stackingImage",out);
        float comentropy2 = computComentropy(out);
        qDebug()<<"1 = "<<comentropy1<<"  2 = "<<comentropy2;
        QString text = QString("original image's comentropy = %1 and stacking image's comentropy is %2").arg(comentropy1).arg(comentropy2);
        QMessageBox::information(this,"comentropy",text);
    });
    QAction* laplaceOperator = encodeAndDivisionMenu->addAction("laplaceOperator");
    connect(laplaceOperator,&QAction::triggered,[&](){
        Mat originalMat;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            return;
        }
        int threshold = QInputDialog::getInt(this,"comentropy","Plz input threadhold",5,0,255);
        originalMat = imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE);
        imshow("beforeLaplce",originalMat);
        float comen1 = computComentropy(originalMat);
        Mat showMat,out;
        originalMat.copyTo(showMat);
        //uchar theradhold = 100;
        for (int i=1;i<originalMat.rows-1;i++) {
            for(int j=1;j<originalMat.cols-1;j++) {
                showMat.at<uchar>(i,j) = abs(originalMat.at<uchar>(i+1,j)+originalMat.at<uchar>(i-1,j)+originalMat.at<uchar>(i,j+1)+originalMat.at<uchar>(i,j-1)-4*originalMat.at<uchar>(i,j));
                //if(showMat.at<uchar>(i,j))
                if(showMat.at<uchar>(i,j)<threshold){
                    showMat.at<uchar>(i,j) = 0;
                }else{
                    showMat.at<uchar>(i,j) = 255;
                }
            }
        }
        imshow("afterLaplace",showMat);
        out = showMat.clone();
        for (int i=0;i<originalMat.rows;i++) {
            for(int j=0;j<originalMat.cols;j++){
                originalMat.at<uchar>(i,j) = abs(255-originalMat.at<uchar>(i,j));
                out.at<uchar>(i,j) = showMat.at<uchar>(i,j)==0?255:originalMat.at<uchar>(i,j);
            }
        }
        //addWeighted(showMat,0.5,originalMat,0.5,0,out);
        imshow("stackingImage",out);
        float comen2 = computComentropy(out);
        QString text = QString("originalImage's comentropy is %1 and stacking image's comentropy is %2").arg(comen1).arg(comen2);
        QMessageBox::information(this,"comentropy",text);
    });
    QAction* regionGrowing = encodeAndDivisionMenu->addAction("regionGrowing");
    connect(regionGrowing,&QAction::triggered,[&](){
        struct corrds{
            corrds(int _x,int _y):x(_x),y(_y){
            }
            int x,y;
        };

        Mat originalMat;
        int threshold = QInputDialog::getInt(this,"threshold","noLabel");
        if(threshold==0){
            threshold = 5;// Default value is  5
        }
        //qDebug()<<threshold;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            return;
        }
        originalMat = imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE);
        imshow("beforeRegionGrowing",originalMat);
        int **state = new int*[originalMat.rows];
        for(int i=0;i<originalMat.rows;i++){
            state[i] = new int[originalMat.cols];
        }
        for(int i=0;i<originalMat.rows;i++){
            for(int j=0;j<originalMat.cols;j++){
                state[i][j] = 0;
            }
        }
        //Edge case
        for(int i=0;i<originalMat.rows;i++){
            state[i][0]=state[i][originalMat.cols-1]=257;
        }
        for(int j=0;j<originalMat.cols;j++){
            state[0][j]=state[originalMat.rows-1][j]=257;
        }
        vector<corrds> pixels;
        int index = 1;//region index
        for (int i=1;i<originalMat.rows-1;i++) {
            for(int j=1;j<originalMat.cols-1;j++){
                if(state[i][j]==0)// Haven't assign region
                {
                    state[i][j] = index;
                    if(state[i-1][j-1]==0 && abs(originalMat.at<uchar>(i-1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i-1][j-1] = index;
                        pixels.push_back(corrds(i-1,j-1));
                    }
                    if(state[i-1][j]==0 && abs(originalMat.at<uchar>(i-1,j)-originalMat.at<uchar>(i,j))<threshold){
                        state[i-1][j] = index;
                        pixels.push_back(corrds(i-1,j));
                    }
                    if(state[i-1][j+1]==0 && abs(originalMat.at<uchar>(i-1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i-1][j+1] = index;
                        pixels.push_back(corrds(i-1,j+1));
                    }
                    if(state[i][j-1]==0 && abs(originalMat.at<uchar>(i,j-1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i][j-1] = index;
                        pixels.push_back(corrds(i,j-1));
                    }
                    if(state[i][j]==0 && abs(originalMat.at<uchar>(i,j)-originalMat.at<uchar>(i,j))<threshold){
                        state[i][j] = index;
                        pixels.push_back(corrds(i,j));
                    }
                    if(state[i][j+1]==0 && abs(originalMat.at<uchar>(i,j+1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i][j+1] = index;
                        pixels.push_back(corrds(i,j+1));
                    }
                    if(state[i+1][j-1]==0 && abs(originalMat.at<uchar>(i+1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i+1][j-1] = index;
                        pixels.push_back(corrds(i+1,j-1));
                    }
                    if(state[i+1][j]==0 && abs(originalMat.at<uchar>(i+1,j)-originalMat.at<uchar>(i,j))<threshold){
                        state[i+1][j] = index;
                        pixels.push_back(corrds(i+1,j));
                    }
                    if(state[i+1][j+1]==0 && abs(originalMat.at<uchar>(i+1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                        state[i+1][j+1] = index;
                        pixels.push_back(corrds(i+1,j+1));
                    }
                    index++;
                    while(pixels.empty()==false){
                        corrds temp = pixels.back();
                        int i = temp.x;
                        int j = temp.y;
                        int index = state[i][j];
                        pixels.pop_back();
                        if(state[i-1][j-1]==0 && abs(originalMat.at<uchar>(i-1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i-1][j-1] = index;
                            pixels.push_back(corrds(i-1,j-1));
                        }
                        if(state[i-1][j]==0 && abs(originalMat.at<uchar>(i-1,j)-originalMat.at<uchar>(i,j))<threshold){
                            state[i-1][j] = index;
                            pixels.push_back(corrds(i-1,j));
                        }
                        if(state[i-1][j+1]==0 && abs(originalMat.at<uchar>(i-1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i-1][j+1] = index;
                            pixels.push_back(corrds(i-1,j+1));
                        }
                        if(state[i][j-1]==0 && abs(originalMat.at<uchar>(i,j-1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i][j-1] = index;
                            pixels.push_back(corrds(i,j-1));
                        }
                        if(state[i][j]==0 && abs(originalMat.at<uchar>(i,j)-originalMat.at<uchar>(i,j))<threshold){
                            state[i][j] = index;
                            pixels.push_back(corrds(i,j));
                        }
                        if(state[i][j+1]==0 && abs(originalMat.at<uchar>(i,j+1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i][j+1] = index;
                            pixels.push_back(corrds(i,j+1));
                        }
                        if(state[i+1][j-1]==0 && abs(originalMat.at<uchar>(i+1,j-1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i+1][j-1] = index;
                            pixels.push_back(corrds(i+1,j-1));
                        }
                        if(state[i+1][j]==0 && abs(originalMat.at<uchar>(i+1,j)-originalMat.at<uchar>(i,j))<threshold){
                            state[i+1][j] = index;
                            pixels.push_back(corrds(i+1,j));
                        }
                        if(state[i+1][j+1]==0 && abs(originalMat.at<uchar>(i+1,j+1)-originalMat.at<uchar>(i,j))<threshold){
                            state[i+1][j+1] = index;
                            pixels.push_back(corrds(i+1,j+1));
                        }
                    }
                }
            }
        }
        //qDebug()<<"Done!";
        for(int i=1;i<originalMat.rows;i++){
            for(int j=1;j<originalMat.cols;j++){
                if(state[i][j]!=257){
                    originalMat.at<uchar>(i,j)=state[i][j];
                }
            }
        }
        imshow("afterRegionGrowing",originalMat);
    });

    QAction* iterativeThreshold = encodeAndDivisionMenu->addAction("iterativeThreshold");
    connect(iterativeThreshold,&QAction::triggered,[&](){
        Mat originalMat;
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "../",
                                                        tr("Images (*.png *.xpm *.jpg *.*)"));
        if(fileName.isEmpty()){
            return;
        }
        originalMat = imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE);
        imshow("beforeIterative",originalMat);
        float T1 = 127.0,T2 = 255.0,t=0.5;
        int grayScale[255] = {0};
        int rows = originalMat.rows;
        int cols = originalMat.cols*originalMat.channels();
        for(int i=0;i<rows;i++){
            uchar* tempRow = originalMat.ptr<uchar>(i);
            for(int j=0;j<cols;j++){
                grayScale[tempRow[j]]++;
            }
        }
        while(1){
            float miu1 = 0,miu2 = 0;
            int count1 = 0,count2 = 0;
            for (int i=0;i<255;i++) {
                if(!grayScale[i]) continue;
                if(i<=T1){
                    miu1+=grayScale[i]*i;
                    count1+=grayScale[i];
                }else{
                    miu2+=grayScale[i]*i;
                    count2+=grayScale[i];
                }
            }
            T2 = ((miu1/(float)count1)+(miu2/(float)count2))*0.5;
            if(abs(T1-T2)>t){
                T1 = T2;
            }else{
                break;
            }
        }
        //qDebug()<<"T1 = "<<T1<<" T2 = "<<T2;
        for(int i=0;i<rows;i++){
            uchar* tempRow = originalMat.ptr<uchar>(i);
            for(int j=0;j<cols;j++){
                if(tempRow[j]>=T1){
                    tempRow[j]=255;
                }
            }
        }
        imshow("afterIterative",originalMat);
    });

    QVBoxLayout* vBoxLayOut = new QVBoxLayout(this);
    QHBoxLayout* hBoxLayOut = new QHBoxLayout;
    QHBoxLayout* menuLayOut = new QHBoxLayout;

    menuLayOut->addWidget(menuBar);

    //hBoxLayOut->addWidget(LinearMapBtn);
    hBoxLayOut->setObjectName("hLayout");
    hBoxLayOut->addWidget(setter);
    hBoxLayOut->addStretch(1);
    hBoxLayOut->addWidget(originalLabel);
    hBoxLayOut->addStretch(3);
    hBoxLayOut->addWidget(processedLabel);

    //qDebug()<<"processLabel's parent is "<<processedLabel->parent()->objectName();
    setter->show();
    //vBoxLayOut->setSizeConstraint(SizeConstraint())
    //vBoxLayOut.
    vBoxLayOut->addLayout(menuLayOut);
    vBoxLayOut->addStretch(1);
    vBoxLayOut->addLayout(hBoxLayOut);
    vBoxLayOut->addStretch(5);

    this->setLayout(vBoxLayOut);

    connect(this,&Widget::captureSignal,this,&Widget::OnCaptureSignal);



    //FFT Processing:
    //1.Create some MyComplex<double> 1*cols arrays to store data of img
    //2.FFT these arrays and store the result in an Mat temp
    //3.Transposition the Mat temp and repeat step 1 and 2
    //4.Transposition the Mat temp again and get result

}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent *event)
{
    m_painter.begin(this);
    QColor shadowColor = QColor(100, 0, 0, 100);                      //阴影颜色设置;
    m_painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));    //设置画笔;
    if(isCapture==true){
        QRect selectedRect = getRect(m_beginMouse, m_endMouse);
        //m_capturePixmap = m_loadPixmap.copy(selectedRect);
        //m_painter.drawPixmap(selectedRect.topLeft(), m_capturePixmap);
        m_painter.drawRect(selectedRect);
        m_painter.fillRect(selectedRect, shadowColor);
    }
    if(isMove==true){
        QPoint offset = -m_beginMouse+m_endMouse;
        processedLabel->move(processedLabel->rect().topLeft()+offset);
        //processedLabel->setGeometry(processedLabel->rect().+offset.x(),processedLabel->y()+offset.y(),processedLabel->rect().width(),processedLabel->rect().height());
    }
    m_painter.end();  //重绘结束;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(isCapture==true){
        m_beginMouse = event->pos();
        //QPoint offset = m_beginMouse - m_endMouse;
        m_globalBeginMouse = event->globalPos();

    }
    if(isMove==true){
        m_beginMouse = event->pos();
    }
    //return mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(isCapture==true){
        m_endMouse = event->pos();
        m_globalEndMouse = event->globalPos();
        update();
    }
    if(true==isMove){
        m_endMouse = event->pos();

        update();
    }
    //return mouseMoveEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(isCapture==true){
        m_endMouse = event->pos();
        m_globalEndMouse = event->globalPos();
        //m_isMousePress = false;
        isCapture=false;
        update();
        emit captureSignal();
    }
    if(isMove==true){
        m_endMouse = event->pos();
        isMove=false;
        update();
        m_beginMouse = event->pos();
    }
    //return mouseReleaseEvent(event);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_S){
        if(afterPixmap.isNull()){
            QMessageBox::information(this,"Error","You didn't open any image!");
            return;
        }
        //QImage img2 = CVS::cvMatToQImage(img);//img.toImage();

        QString filename = QFileDialog::getSaveFileName(this,
            tr("Save Image"),
            "",
            tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
        if (filename.isEmpty())
        {
            return;
        }
        else
        {
            if (!(afterPixmap.save(filename))) //保存图像
            {
                QMessageBox::information(this,
                    tr("Failed to save the image"),
                    tr("Failed to save the image!"));
                return;
            }
            QMessageBox::information(this,tr("Success!"),tr("Save successfully!"));
            return;
        }
    }

}

/*
 将已有数组赋给Mat矩阵的方法：
 cv::Mat M = cv::Mat(height,width,<Type>,data)
 例：
 float K[3][3] = {fc[0], 0, cc[0], 0, fc[1], cc[1], 0, 0, 1};    //摄像机内参数矩阵K
 cv::Mat mK = cv::Mat(3,3,CV_32FC1,K);    //内参数K Mat类型变量
*/
void Widget::FFT(MyComplex<double> *TD, MyComplex<double> *FD, int r){
    long	count;				// 付立叶变换点数
    int		i, j, k;				// 循环变量
    int		bfsize, p;
    double	angle;				// 角度
    MyComplex<double>* W, * X1, * X2, * X;

    count = 1 << r;				// 计算付立叶变换点数

    // 分配运算所需存储器
    W = new MyComplex<double>[count / 2];
    X1 = new MyComplex<double>[count];
    X2 = new MyComplex<double>[count];

    // 计算加权系数
    for (i = 0; i < count / 2; i++)
    {
        angle = -i * PI * 2 / count;
        W[i] = MyComplex<double>(cos(angle),sin(angle));
    }

    // 将时域点写入X1
    memcpy(X1, TD, sizeof(MyComplex<double>) * count);

    // 采用蝶形算法进行快速付立叶变换
    for (k = 0; k < r; k++)//k为蝶形运算的级数
    {
        for (j = 0; j < 1 << k; j++)
        {
            bfsize = 1 << (r - k);//做蝶形运算两点间距离
            for (i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
                X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2])
                        * W[i * (1 << k)];
            }
        }
        X = X1;
        X1 = X2;
        X2 = X;
    }
    // 重新排序
    for (j = 0; j < count; j++)
    {
        p = 0;
        for (i = 0; i < r; i++)
        {
            if (j & (1 << i))
            {
                p += 1 << (r - i - 1);
            }
        }
        FD[j] = X1[p];
    }
    delete W;
    delete X1;
    delete X2;
}

void Widget::Fourier(int nr,int nc,MyComplex<double>*tdComplex,MyComplex<double>*fdComplex){
    int ncPoint = getPowerPoint(nc);
    int nrPoint = getPowerPoint(nr);
    /*FFT in X*/
    for(int i = 0; i < nr; i++)
    {
        Widget::FFT(&tdComplex[nc * i], &fdComplex[nc * i], ncPoint);
    }
    /*Translation fdComplex*/
    for(int i = 0;i < nr;i++){
        for (int j = 0;j < nc;j++) {
            tdComplex[nr*j+i]=fdComplex[nc*i+j];
        }
    }
    /*FFT again*/
    for(int j = 0; j < nc; j++){
        Widget::FFT(&tdComplex[j*nc],&fdComplex[j*nc],nrPoint);// 注意点数的变化，这里长宽一样所以不用改
    }

    /*Store intermedia result in this->fdComplex*/
    for(int i = 0; i < nr; i++){
        for (int j = 0; j < nc; j++) {
            this->fdComplex[nc * i + j] = fdComplex[nc * i + j];
        }

    }
}

void Widget::InverseFourier(int nr, int nc, MyComplex<double> *tdComplex, MyComplex<double> *fdComplex) {
    MyComplex<double>*tempWork = new MyComplex<double>[sizeof(MyComplex<double>)*(static_cast<int>(nr))*(static_cast<int>(nc))];
    //get conjugate
    MyComplex<double>* tmp;
    for(int i = 0; i < nr; i++) {
        for(int j = 0; j < nc; j++) {
            tmp = &fdComplex[nc * i + j] ;
            tempWork[nc * i + j].rel=tmp->rel;
            tempWork[nc * i + j].img=-tmp->img;
        }
    }
    //fourier
    Widget::Fourier(nr,nc,tempWork,tdComplex);
    //get conjugate and divide a coefficient
    for(int i = 0; i < nr; i++) {
        for(int j = 0; j < nc; j++) {
            tmp = &tdComplex[nc * i + j];
            tdComplex[nc * i + j].rel=tmp->rel/(double)(nr*nc);
            tdComplex[nc * i + j].img=-tmp->img/(double)(nr*nc);
        }
    }
    delete[] tempWork;
}

void Widget::DisCosine(int nr, int nc, MyComplex<double> *tdComplex, MyComplex<double> *fdComplex){
    int i,j;
    int ncPoint = getPowerPoint(nc);
    int nrPoint = getPowerPoint(nr);
    for(i = 0; i < nr; i++)
    {
        FDCT(&tdComplex[nc * i], &fdComplex[nc * i], ncPoint);// 对y方向进行离散余弦变换 第三个参数是  宽度的指数
    }
    for (i = 0; i < nr; i++)// 保存计算结果，转置，准备进行x方向的离散余弦变换
    {
        for (j = 0; j < nc; j++)
        {
            tdComplex[j * nr + i] = fdComplex[j + nc * i];
        }
    }
    for(j = 0; j < nc; j++)
    {
        FDCT(&tdComplex[j * nr], &fdComplex[j * nr], nrPoint);// 对x方向进行离散余弦变换 第三个参数是  高度的指数
    }
    /* Store result in this->fdComplex*/
    for(int i = 0; i < nr; i++){
        for (int j = 0; j < nc; j++) {
            this->fdComplex[nc * i + j] = fdComplex[nc * i + j];
        }
    }
}

void Widget::InverseDisCosine(int nr, int nc, MyComplex<double> *tdComplex, MyComplex<double> *fdComplex){
    DisCosine(nr,nc,fdComplex,tdComplex);
}

void Widget::FDCT(MyComplex<double> *TD, MyComplex<double> *FD, int r){
    int i;                 //行循环变量
    long	dotCount;// 离散余弦变换点数
    double	dTemp;// 临时变量
    MyComplex<double> *temReg;
    dotCount = 1<<r;// 计算离散余弦变换点数
    temReg = new MyComplex<double>[sizeof(MyComplex<double>) *dotCount*2];// 分配内存
    memset(temReg, 0, sizeof(MyComplex<double>) * dotCount * 2);// 赋为0
    for(i=0;i<dotCount;i++)// 将时域点写入数组temReg
    {
        temReg[i].rel=TD[i].rel;
        temReg[i].img=0;
    }
    FFT(temReg,temReg,r+1);// 调用快速付立叶变换
    dTemp = 1/sqrt((double)dotCount);// 调整系数
    FD[0].rel = temReg[0].rel*dTemp;// 求pFd[0]
    dTemp *= sqrt(2.0f);
    for(i = 1; i < dotCount; i++)// 求pFd[u]
    {
        FD[i].rel=(temReg[i].rel* cos(i*PI/(dotCount*2)) + temReg[i].img* sin(i*PI/(dotCount*2))) * dTemp;
    }
    delete temReg;// 释放内存
}

void Widget::Walsh(int nr, int nc, MyComplex<double> *tdComplex, MyComplex<double> *fdComplex){
    int i,j;
    int ncPoint = getPowerPoint(nc);
    int nrPoint = getPowerPoint(nr);
    for(i = 0; i < nr; i++){
        FWT(&tdComplex[nc * i], &fdComplex [nc * i], ncPoint);// 对y方向进行沃尔什-哈达玛变换 最后一个参数是 宽的次数
    }
    // 保存计算结果,转置
    for(i = 0; i < nr; i++) {
        for(j = 0; j < nc; j++) {
            tdComplex[j * nr + i] = fdComplex[j + nc * i];
        }
    }
    for(j = 0; j < nc; j++) {
        FWT(&tdComplex[j * nr], &fdComplex[j * nr], nrPoint);// 对x方向进行沃尔什-哈达玛变换
    }

    for(int i = 0; i < nr; i++){
        for (int j = 0; j < nc; j++) {
            this->fdComplex[nc * i + j] = fdComplex[nc * i + j];
        }
    }
}

void Widget::FWT(MyComplex<double> *TD, MyComplex<double> *FD, int r){
    int	dotCount;// 沃尔什-哈达玛变换点数
    int	i,j,k;// 循环变量
    int	bfsize,p;// 中间变量
    MyComplex<double> *temReg1,*temReg2,*temReg;
    dotCount = 1 << r;// 计算快速沃尔什变换点数
    temReg1 = new MyComplex<double>[dotCount];// 分配运算所需的数组
    temReg2 = new MyComplex<double>[dotCount];
    memcpy(temReg1, TD, sizeof(MyComplex<double>) * dotCount);// 将时域点写入数组temReg1
    for(k = 0; k < r; k++)// 蝶形运算
    {
        for(j = 0; j < 1<<k; j++)
        {
            bfsize = 1 << (r-k);
            for(i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                temReg2[i + p] = temReg1[i + p] + temReg1[i + p + bfsize / 2];
                temReg2[i + p + bfsize / 2] = temReg1[i + p] - temReg1[i + p + bfsize / 2];
            }
        }
        temReg = temReg1;// 互换temReg1和temReg2
        temReg1 = temReg2;
        temReg2 = temReg;
    }
    for(j = 0; j < dotCount; j++)// 调整系数
    {
        p = 0;
        for(i = 0; i < r; i++)
        {
            if (j & (1<<i))
            {
                p += 1 << (r-i-1);
            }
        }
        FD[j].rel = temReg1[p].rel / (double)dotCount;
        //qDebug()<<FD[j].rel;
        //FD[j].img = temReg1[p].img / (double)dotCount;
    }
    delete temReg1;// 释放内存
    delete temReg2;
}

int Widget::getPowerPoint(int x) {
    int ans = 0;
    int tmp = 1;
    while(2*tmp<=x){
        ans++;
        tmp*=2;
    }
    return ans;
}

int Widget::getClosestPower(int x) {
    int ans=1;
    while(ans<x){
        ans*=2;
    }
    return ans;
}

int Widget::linearTrans(int a, int b, int c, int d,int f)
{
    return (int)((((float)d-(float)c)/((float)b-(float)a))*((float)f-(float)a)+(float)c);
}

int Widget::logTrans(float a, float b,float c,int pixel)
{
    return (int)((log((double)(pixel+1)))/(b*log(c))+a+0.5);
}

QRect Widget::getRect(const QPoint &beginPoint, const QPoint &endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    // 避免宽或高为零时拷贝截图有误;
    // 可以看QQ截图，当选取截图宽或高为零时默认为2;
    if (selectedRect.width() == 0)
    {
        selectedRect.setWidth(1);
    }
    if (selectedRect.height() == 0)
    {
        selectedRect.setHeight(1);
    }

    return selectedRect;
}

Mat AddNoise(Mat img,double SNR ){
    Mat outImage;
    outImage.create(img.rows,img.cols,img.type());
    int SP = img.rows*img.cols;
    int NP = SP*(1-SNR);
    outImage = img.clone();
    for(int i=0; i<NP; i++) {

        int x = (int)(rand()*1.0/RAND_MAX* (double)img.rows);
        int y = (int)(rand()*1.0/RAND_MAX* (double)img.cols);
        int r = rand()%2;
        if(r){
            outImage.at<uchar>(x, y)=0;
        }
        else{
            outImage.at<uchar>(x, y)=255;
        }

    }
    return outImage;
}

double Widget::generateGaussianNoise(double mu, double sigma)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;
    double U1,U2;
    if ( phase == 0 ) {
        do {
            U1 = (double)rand() / RAND_MAX;
            U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else{
        X = V2 * sqrt(-2 * log(S) / S);
    }
    phase = 1 - phase;
    return mu+sigma*X;
}

Mat Widget::AddNoise(Mat img, double mu, double sigma, int k)
{
    Mat outImage;
    int PixcelMax = 255;
    int PixcelMin = 0;
    qDebug()<<"mu sigma k = "<<mu<<"  "<<sigma<<"  "<<k;
    outImage.create(img.rows,img.cols,img.type());
    for(int x=0;x<img.rows;x++){
        for(int y=0;y<img.cols;y++){
            double temp = img.at<uchar>(x, y)
                    +k*generateGaussianNoise(mu,sigma);
            if(temp>PixcelMax)
                temp=PixcelMax;
            else if(temp<PixcelMin)
                temp=PixcelMin;
            outImage.at<uchar>(x, y) = temp;
        }
    }
    return outImage;
}

Mat Widget::AddNoise(Mat img, double SNR)
{
    Mat outImage;
    outImage.create(img.rows,img.cols,img.type());
    if(img.type()==CV_8U||img.type()==CV_8UC1) qDebug()<<"cv_8u";
    int SP = img.rows*img.cols;
    int NP = SP*(1-SNR);
    outImage = img.clone();
    for(int i=0; i<NP; i++) {
        int x = (int)(rand()*1.0/RAND_MAX* (double)img.rows);
        int y = (int)(rand()*1.0/RAND_MAX* (double)img.cols);
        int r = rand()%2;
        if(x>=img.rows||y>=img.cols) continue;
        if(r){
            outImage.at<uchar>(x, y)=0;
        }
        else{
            outImage.at<uchar>(x, y)=255;
        }
    }
    qDebug()<<"impulseNoise is over!";
    return outImage;
}

bool Widget::cvBGR2HSI(Mat& original)
{
    if(original.type()!=16){
        QMessageBox::warning(this,"format error","input mat is not CV_8UC3");
        return false;
    }
    int row = original.rows;
    int col = original.cols*original.channels();
    for(int i=0;i<row;i++){
        uchar* tempRow = original.ptr<uchar>(i);
        for(int j=0;j<col;j+=3){
            uchar B = tempRow[j];
            uchar G = tempRow[j+1];
            uchar R = tempRow[j+2];
            uchar I = (uchar)((B+G+R)/3.0f);
            uchar S = 1.-3.0f/(B+R+G)*std::min(B,std::min(G,R));
            float theta = acos(((R-G+R-B)/2.0f)/sqrt((R-G)*(R-G)+(R-G)*(G-B)));
            uchar H = G>=B ? (uchar)theta : 360-(uchar)theta;
            tempRow[j]=H;
            tempRow[j+1]=S;
            tempRow[j+2]=I;
        }
    }
    //QMessageBox::warning(this,"error","frequency statistic error");
    return true;
}

vector<myNode::Node*> Widget::statisticFrequency(const Mat &original, int* arr)
{
    vector<myNode::Node*> myList;
    if(original.type()!=16){
        //qDebug()<<"input type is "<<original.type();
        return myList;
    }
    Mat tempMat;
    original.copyTo(tempMat);
    int* statisticArr = new int[256]();
    for(int i=0;i<tempMat.rows;i++){
        uchar*tempRow = tempMat.ptr<uchar>(i);
        for (int j=0;j<tempMat.cols*tempMat.channels();j+=3) {
            //if(tempRow[j+2]>255) {qDebug()<<"index out of boundary "<<tempRow[j+2];continue;}
            statisticArr[tempRow[j+2]]++;// Only statistic I compoments
        }
    }
    for(int i=0;i<256;i++){
        if(statisticArr[i]!=0){
            //qDebug()<<statisticArr[i];
            myList.push_back(new myNode::Node(i,statisticArr[i]));
        }
        arr[i] = statisticArr[i];
    }

    //qDebug()<<"myList.size() = "<<myList.size();
    return myList;
}

bool Widget::compressionAndEfficiency(int *arr)
{
    if(m_map==nullptr ||m_map->empty()) return false;
    double H,R,eta,Ls,Ld,Pr;//Definitions are followed http://www.docin.com/p-122927560.html
    int sum = 0;
    for (int i=0;i<256;i++) {
        sum+=arr[i];
    }
    double* percentage = new double[256];
    for (int i=0;i<256;i++) {
        percentage[i] = arr[i]/(double)sum;
    }
    H=0.0;
    R=0.0;
    Ls=0.0;
    Ld=.0;
    QString tempString;
    QMap<int,std::string>::iterator temp;
    qDebug()<<"percentage is over!";
    for(int i=0;i<256;i++) {
        temp = m_map->find(i);
        if(temp==m_map->end()) continue;
        tempString = QString::fromStdString(temp.value());
        if(tempString.isEmpty()) continue;
        //if(percentage[i]<1e-4) continue;
        H+=(-percentage[i]*log2(percentage[i]));
        //qDebug()<<"value is "<<QString::fromStdString(m_map->find(i).value());
        R += (tempString.length()*percentage[i]);
        Ls+=(QString::number(i,2).length()*arr[i]);
        //qDebug()<<"i is"<<i<<" and binary code is "<<QString::number(i,2)<<" and length is "<<QString::number(i,2).length();
        Ld+=(tempString.length()*arr[i]);
    }
    //qDebug()<<"Information:"<<H;
    qDebug()<<"percentage is over!";
    eta = H/R;
    if(Ls<Ld) qDebug()<<"Ls is "<<Ls<<" Ld is "<<Ld;
    Pr=(Ls-Ld)/Ls*100.0;
    qDebug()<<"eta = "<<eta<<"  Pr = "<<Pr;
    return true;
}

float Widget::computComentropy(Mat &input)
{
    int col = input.cols*input.channels();
    int row = input.rows;
    float* arr = new float[256];
    for(int i=0;i<256;i++){
        arr[i]=0;
    }
    for(int i=0;i<row;i++){
        uchar* tempRow = input.ptr<uchar>(i);
        for(int j=0;j<col;j++){
            arr[tempRow[j]]++;
        }
    }
    for(int i=0;i<256;i++){
        arr[i]/=(float)(col*row);
    }
    float ans = .0f;
    for(int i=0;i<256;i++){
        if(arr[i]!=0.0f){
            ans = ans-arr[i]*(std::log(arr[i])/log(2.0f));
        }
    }
    return ans;
}

void Widget::OnCaptureSignal()
{
    qDebug()<<"process's parent is "<<processedLabel->parent()->objectName();
    QPoint leftTop = processedLabel->mapFromParent(m_beginMouse);
    QPoint rightBottom = processedLabel->mapFromParent(m_endMouse);
    QPixmap before(QPixmap(processedLabel->pixmap()->copy(processedLabel->rect())));
    QPixmap after(QPixmap(processedLabel->pixmap()->copy(getRect(leftTop,rightBottom))));
    processedLabel->setScaledContents(false);
    processedLabel->setPixmap(after);
    QMessageBox:: StandardButton result= QMessageBox::information(NULL, "确认", "截图确认吗？",QMessageBox::Yes|QMessageBox::No);
    switch (result) {
    case QMessageBox::Yes:
        processedLabel->setPixmap(after);
        break;
    case QMessageBox::No:
        processedLabel->setPixmap(before);
        break;
    default:
        break;
    }
}
