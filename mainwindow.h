#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QMessageBox>
#include <QTextStream>
#include <QLabel>
#include <QImage>
#include <QPainter>

using namespace std;

struct RGB{
    uint r;
    uint g;
    uint b;
};

enum YUV_FORMAT{
    NV21_YUV420SP=0,
    NV12_YUV420SP=1,
    NV16_YUV422SP=2,
    NV61_YUV422SP=3,
};

enum color_fmts {
COLOR_FMT_NV12,
    COLOR_FMT_NV21,
    COLOR_FMT_NV12_MVTB,
    COLOR_FMT_NV12_UBWC,
    COLOR_FMT_NV12_BPP10_UBWC,
    COLOR_FMT_RGBA8888,
    COLOR_FMT_RGBA8888_UBWC,
    COLOR_FMT_RGBA1010102_UBWC,
    COLOR_FMT_RGB565_UBWC,
    COLOR_FMT_P010_UBWC,
};

#ifndef MSM_MEDIA_ALIGN
#define MSM_MEDIA_ALIGN(__sz, __align) (((__align) & ((__align) - 1)) ?\
    ((((__sz) + (__align) - 1) / (__align)) * (__align)) :\
    (((__sz) + (__align) - 1) & (~((__align) - 1))))
#endif


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void (MainWindow::*PFunc)(char*, char*, int, int) =NULL;
    explicit MainWindow(QWidget *parent = 0);
    void loadFile(const QString &fileName);
    void YUV2RGB(char Y, char U, char V, struct RGB *rgb);
    ~MainWindow();

private slots:
    int collectFormatinfo();

    void processConvert(QString &filename, int);

    void on_actionOpen_triggered();

    void NV12_2_RGB888(char *pYUV, char *pBGR24, int width, int height);

    void NV21_2_RGB888(char *pYUV, char *pBGR24, int width, int height);

    void NV16_2_RGB888(char *pYUV, char *pRGB24, int width, int height);

    void NV61_2_RGB888(char *pYUV, char *pRGB24, int width, int height);

    void YV12_2_RGB888(char *pYUV, char *pRGB24, int width, int height);

    void YV21_2_RGB888(char *pYUV, char *pRGB24, int width, int height);

    unsigned int VENUS_Y_STRIDE(int color_fmt, int width);
    unsigned int VENUS_UV_STRIDE(int color_fmt, int width);
    unsigned int VENUS_Y_SCANLINES(int color_fmt, int height);
    unsigned int VENUS_UV_SCANLINES(int color_fmt, int height);
    void VenusNV12_2_NV12(char *src, char *dst, int src_width, int src_height);

    void on_SrcFormatCombo_currentIndexChanged(const QString &arg1);

    void on_selectFileBtn_clicked();

    void on_SrcFormatCombo_activated(int index);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
