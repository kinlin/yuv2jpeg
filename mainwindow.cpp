#include "mainwindow.h"
#include "ui_mainwindow.h"

QImage image_NV12(":/NV12.png");
QImage image_NV21(":/NV21.png");
QImage image_NV16(":/NV16.png");
QImage image_NV61(":/NV61.png");
QImage image_YV12(":/YV12.png");
QImage image_YV21(":/YV21.png");



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("ImageConvert inited!");

/*
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0.0, 0.0, 590.0, 200.0);
    QGraphicsEllipseItem * ellipse;
    QPen pen(Qt::black);
    QBrush brush(Qt::blue);
    //ellipse = scene->addEllipse(10,10,100,100,pen, brush);
    scene->addRect(10,10,100,100, pen, brush);
    scene->addRect(-50, -50, 100, 100, pen, brush);

    ui->yuvPatternView->setScene(scene);
    ui->yuvPatternView->setSceneRect(10,10,100,100);

*/
    ui->yuvLabelPic->setScaledContents(true);
    ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_NV21));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
//    QString fileName = QFileDialog::getOpenFileName(this);
//    if (!fileName.isEmpty())
//       loadFile(fileName);
    QMessageBox::warning(this, tr("About"),
                         tr("This is a tool used to convert between different Image Format!"));
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }



    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::YUV2RGB(char Y, char U, char V, struct RGB *rgb) {
    rgb->r = (int)((Y&0xff) + 1.4075 * ((V&0xff)-128));
    rgb->g = (int)((Y&0xff) - 0.3455 * ((U&0xff)-128) - 0.7169*((V&0xff)-128));
    rgb->b = (int)((Y&0xff) + 1.779 *  ((U&0xff)-128));

    rgb->r =(rgb->r<0? 0: (rgb->r>255? 255 : rgb->r) );
    rgb->g =(rgb->g<0? 0: (rgb->g>255? 255 : rgb->g) );
    rgb->b =(rgb->b<0? 0: (rgb->b>255? 255 : rgb->b) );
}


void MainWindow::NV21_2_RGB888(char *pYUV, char *pRGB24, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return;
    const long len = width * height;
    char *vu_buf = pYUV + len;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++){
            int Y=i*width+j;
            int V=i/2*width +(j/2)*2;
            int U=V+1;
            int index=Y*3;
            struct RGB rgb={0};
            YUV2RGB(pYUV[Y], vu_buf[U], vu_buf[V], &rgb);
            pRGB24[index+0] = rgb.r;
            pRGB24[index+1] = rgb.g;
            pRGB24[index+2] = rgb.b;
        }
    }
}

void MainWindow::NV12_2_RGB888(char *pYUV, char *pRGB24, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return;
    const long len = width * height;
    char *vu_buf = pYUV + len;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++){
            int Y=i*width+j;
            int U=i/2*width +(j/2)*2;
            int V=U+1;
            int index=Y*3;
            struct RGB rgb={0};
            YUV2RGB(pYUV[Y], vu_buf[U], vu_buf[V], &rgb);
            pRGB24[index+0] = rgb.r;
            pRGB24[index+1] = rgb.g;
            pRGB24[index+2] = rgb.b;
        }
    }
}




//NV16(YUV422SP)  YUV422  2 plane   (Y), (UV)
//From android graphic:  Used for video
/*
+----------------+
|Y0Y1Y2Y3Y4Y5Y6Y7|
|                |
+----------------+
|U0V0U1V1U2V2U3V3|
|                |
+----------------+
*/
void MainWindow::NV16_2_RGB888(char *pYUV, char *pRGB24, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return;
    const long len = width * height;
    char *vu_buf = pYUV + len;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++){
            int Y=i*width+j;
            int U=i*width +(j/2)*2;
            int V=U+1;
            int index=Y*3;
            struct RGB rgb={0};
            YUV2RGB(pYUV[Y], vu_buf[U], vu_buf[V], &rgb);
            pRGB24[index+0] = rgb.r;
            pRGB24[index+1] = rgb.g;
            pRGB24[index+2] = rgb.b;
        }
    }
}


//NV61(YUV422SP)  YUV422  2 plane   (Y), (VU)
/*
+----------------+
|Y0Y1Y2Y3Y4Y5Y6Y7|
|                |
+----------------+
|V0U0V1U1V2U2V3U3|
|                |
+----------------+
*/
void MainWindow::NV61_2_RGB888(char *pYUV, char *pRGB24, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return;
    const long len = width * height;
    char *vu_buf = pYUV + len;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++){
            int Y=i*width+j;
            int V=i*width +(j/2)*2;
            int U=V+1;
            int index=Y*3;
            struct RGB rgb={0};
            YUV2RGB(pYUV[Y], vu_buf[U], vu_buf[V], &rgb);
            pRGB24[index+0] = rgb.r;
            pRGB24[index+1] = rgb.g;
            pRGB24[index+2] = rgb.b;
        }
    }
}

void MainWindow::YV12_2_RGB888(char *pYUV, char *pRGB24, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return;
    const long len = width * height;
    char *vu_buf = pYUV + len;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++){
            int Y=i*width+j;
            int V=(i/2)*(width/2)+ j/2;
            int U=V+len/4;
            int index=Y*3;
            struct RGB rgb={0};
            YUV2RGB(pYUV[Y], vu_buf[U], vu_buf[V], &rgb);
            pRGB24[index+0] = rgb.r;
            pRGB24[index+1] = rgb.g;
            pRGB24[index+2] = rgb.b;
        }
    }
}

void MainWindow::YV21_2_RGB888(char *pYUV, char *pRGB24, int width, int height) {
    if (width < 1 || height < 1 || pYUV == NULL || pRGB24 == NULL)
        return;
    const long len = width * height;
    char *vu_buf = pYUV + len;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++){
            int Y=i*width+j;
            int U=(i/2)*(width/2)+j/2;
            int V=U+len/4;
            int index=Y*3;
            struct RGB rgb={0};
            YUV2RGB(pYUV[Y], vu_buf[U], vu_buf[V], &rgb);
            pRGB24[index+0] = rgb.r;
            pRGB24[index+1] = rgb.g;
            pRGB24[index+2] = rgb.b;
        }
    }
}

void MainWindow::VenusNV12_2_NV12(char *src, char *dst, int src_width, int src_height) {

    const int u_planar_length = src_width * src_height / 4;  // 没有对齐U部分的长度
    const int y_planar_length = src_width * src_height;  // 没有对齐Y部分的长度
    const int uv_width = src_width / 2;  // 没有对齐U部分的长
    const int uv_height = src_height / 2;  // 没有对齐U部分的高
    const int dst_file_length = y_planar_length + u_planar_length * 2;  // 转成YUV420的长度

    // 对齐Y部分的长度
    const int nv12_y_length = VENUS_Y_STRIDE(COLOR_FMT_NV12, src_width) * VENUS_Y_SCANLINES(COLOR_FMT_NV12, src_height);
    // 对齐U部分的长
    const int nv12_uv_width = VENUS_UV_STRIDE(COLOR_FMT_NV12, src_width);

    for (int i = 0; i < src_width * src_height; i++) {
        dst[i] = src[i];
    }

    for (int i = 0; i < uv_height; i++) {
        for (int j = 0; j < uv_width; j++) {
            dst[y_planar_length + i * uv_width + j] = src[nv12_y_length + i * nv12_uv_width + j * 2];
            dst[y_planar_length + u_planar_length + i * uv_width + j] =
                src[nv12_y_length + i * nv12_uv_width + j * 2 + 1];
        }
    }

}

unsigned int MainWindow::VENUS_Y_STRIDE(int color_fmt, int width)
{
    unsigned int alignment, stride = 0;
    if(!width)
        goto invalid_input;
    switch (color_fmt) {
    case COLOR_FMT_NV21:
    case COLOR_FMT_NV12:
    case COLOR_FMT_NV12_MVTB:
    case COLOR_FMT_NV12_UBWC:
        alignment = 128;
        stride = MSM_MEDIA_ALIGN(width, alignment);
        break;
    case COLOR_FMT_NV12_BPP10_UBWC:
        alignment = 256;
        stride = MSM_MEDIA_ALIGN(width, 192);
        stride = MSM_MEDIA_ALIGN(stride * 4/3, alignment);
        break;
    case COLOR_FMT_P010_UBWC:
        alignment = 256;
        stride = MSM_MEDIA_ALIGN(width * 2, alignment);
        break;
    default:
        break;
    }
invalid_input:
    return stride;
}

unsigned int MainWindow::VENUS_UV_STRIDE(int color_fmt, int width)
{
    unsigned int alignment, stride = 0;

    if (!width)
        goto invalid_input;

    switch (color_fmt) {
    case COLOR_FMT_NV21:
    case COLOR_FMT_NV12:
    case COLOR_FMT_NV12_MVTB:
    case COLOR_FMT_NV12_UBWC:
        alignment = 128;
        stride = MSM_MEDIA_ALIGN(width, alignment);
        break;
    case COLOR_FMT_NV12_BPP10_UBWC:
        alignment = 256;
        stride = MSM_MEDIA_ALIGN(width, 192);
        stride = MSM_MEDIA_ALIGN(stride * 4/3, alignment);
        break;
    case COLOR_FMT_P010_UBWC:
        alignment = 256;
        stride = MSM_MEDIA_ALIGN(width * 2, alignment);
        break;
    default:
        break;
    }
invalid_input:
    return stride;
}

unsigned int MainWindow::VENUS_Y_SCANLINES(int color_fmt, int height)
{
    unsigned int alignment, sclines = 0;

    if (!height)
        goto invalid_input;

    switch (color_fmt) {
    case COLOR_FMT_NV21:
    case COLOR_FMT_NV12:
    case COLOR_FMT_NV12_MVTB:
    case COLOR_FMT_NV12_UBWC:
        alignment = 32;
        break;
    case COLOR_FMT_NV12_BPP10_UBWC:
    case COLOR_FMT_P010_UBWC:
        alignment = 16;
        break;
    default:
        return 0;
    }
    sclines = MSM_MEDIA_ALIGN(height, alignment);
invalid_input:
    return sclines;
}

unsigned int MainWindow::VENUS_UV_SCANLINES(int color_fmt, int height)
{
    unsigned int alignment, sclines = 0;

    if (!height)
        goto invalid_input;

    switch (color_fmt) {
    case COLOR_FMT_NV21:
    case COLOR_FMT_NV12:
    case COLOR_FMT_NV12_MVTB:
    case COLOR_FMT_NV12_BPP10_UBWC:
    case COLOR_FMT_P010_UBWC:
        alignment = 16;
        break;
    case COLOR_FMT_NV12_UBWC:
        alignment = 32;
        break;
    default:
        goto invalid_input;
    }

    sclines = MSM_MEDIA_ALIGN(height / 2, alignment);

invalid_input:
    return sclines;
}

void MainWindow::on_selectFileBtn_clicked()
{
    int w,h;

    ui->statusBar->showMessage("Imaging Processing......");
    QString yuvFilters = "YUV file(*.yuv)";
    QStringList fileLists = QFileDialog::getOpenFileNames(this, tr("please select yuv files"),
                                                          QDir::currentPath(),
                                                          tr("YUV(*.yuv);;JPEG (*.jpg *.jpeg);;TIFF (*.tif);;All files (*.*)" ),
                                                          &yuvFilters);
    if(fileLists.size()<=0){
        QMessageBox::warning(this, tr("Warning"),
                             tr("You did not select a image."));
        ui->statusBar->showMessage("Imaging Process Aborted！");
        return;
    }

    int srcFormat = collectFormatinfo();
    for (int i = 0; i < fileLists.size(); ++i){
         printf("\n%s\n",fileLists.at(i).toLocal8Bit().constData());
        QString filename=fileLists.at(i);
        if (!filename.isEmpty()){
            processConvert(filename, srcFormat);
        }
    }
}

void MainWindow::processConvert(QString &FileNames, int srcYuvFormat){
    int w;
    int h;
    float yuv_coef=1.5;


    QString FileName = FileNames;
    printf("\nprocessConvert %s\n", FileName.toLocal8Bit().constData());
    QFileInfo fileInfo = QFile(FileName);
    QString fname = fileInfo.completeBaseName();
    QString newFileName = fileInfo.path()+"/"+fileInfo.completeBaseName()+".yuv.jpg";

    // get width and height from filenames
    QString pattern("([0-9]{2,4})x([0-9]{2,4})");
    QRegExp rx(pattern);
    int pos = fname.indexOf(rx);
    QStringList result = rx.capturedTexts();
    QString wStr = result.at(1);
    QString hStr = result.at(2);



    if( wStr.toInt()>0 && hStr.toInt()>0){
        w=wStr.toInt();
        h=hStr.toInt();
    } else {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read picture width and height."));
        return;
    }

    if(srcYuvFormat==0){
        yuv_coef = 1.5;
        PFunc= &MainWindow::NV21_2_RGB888;
    }else if(srcYuvFormat==1){
        yuv_coef = 1.5;
        PFunc= &MainWindow::NV12_2_RGB888;
    }else if(srcYuvFormat==2){
        yuv_coef = 2.0;
        PFunc= &MainWindow::NV16_2_RGB888;
    }else if(srcYuvFormat==3){
        yuv_coef = 2.0;
        PFunc= &MainWindow::NV61_2_RGB888;
    }else if(srcYuvFormat==4){
        yuv_coef = 1.5;
        PFunc= &MainWindow::YV12_2_RGB888;
    }else if(srcYuvFormat==5){
        yuv_coef = 1.5;
        PFunc= &MainWindow::YV21_2_RGB888;
    }

    //will change follow yuv420 or yuv422
    char *yuvBuf = (char*)malloc(w*h*yuv_coef);
    char *rgbBuf = (char*)malloc(w*h*3);


    if (FileName.isEmpty()) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read FileName."));
    }

    QFile file(FileName);

    QString text;
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(FileName), file.errorString()));
        return;
    } else {
        text = file.read(yuvBuf, w*h*yuv_coef);
    }
    file.close();

    //NV21_2_RGB888(yuvBuf, rgbBuf, w, h);
    (*this.*PFunc)(yuvBuf, rgbBuf, w, h);

    QImage tmpImage =QImage((uchar *)rgbBuf, w, h, QImage::Format_RGB888);
    tmpImage.save(newFileName, "JPG", 70);

    free(yuvBuf);
    free(rgbBuf);

    ui->statusBar->showMessage("Imaging Process done ! You can start new convert task!");
}


int MainWindow::collectFormatinfo(){
    return ui->SrcFormatCombo->currentIndex();
}

void MainWindow::on_SrcFormatCombo_activated(int index)
{
    printf("will this called at the beginning?");
    if(index==0){
       ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_NV21));
    }else if(index==1){
       ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_NV12));
    }else if(index==2){
       ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_NV16));
    }else if(index==3){
       ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_NV61));
    }else if(index==4){
       ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_YV12));
    }else if(index==5){
       ui->yuvLabelPic->setPixmap(QPixmap::fromImage(image_YV21));
    }
}

void MainWindow::on_SrcFormatCombo_currentIndexChanged(const QString &arg1)
{
    //if comboBox text change, we need to do sth
    printf("comboBox changed!");
}
