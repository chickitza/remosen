#include "remosen.h"


remosen::remosen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.progressBar->setValue(0);
}

remosen::~remosen()
{}

void remosen::on_inPushButton_clicked()
{
    qDebug() << "ok in";
    this->inFilePath = QFileDialog::getOpenFileName(this, QStringLiteral("请选择输入影像"), "", tr("TIFF(*.tif *.tiff);;JPEG(*.jpg *.jpeg);;" "All files(*)"));
    ui.inLineEdit->setText(this->inFilePath);
}

void remosen::on_outPushButton_clicked()
{
    qDebug() << "ok in";
    this->outFilePath = QFileDialog::getSaveFileName(this, QStringLiteral("保存输出影像路径"), "", tr("TIFF(*.tif *.tiff);;JPEG(*.jpg *.jpeg);;" "All files(*)"));
    ui.outLineEdit->setText(this->outFilePath);
}

void remosen::on_proPushButton_clicked()
{
    //for (int i = 1; i < 101; i++)
    //{
    //    Sleep(1000);
    //    ui.progressBar->setValue(i);
    //}
    // 
    //// 驱动注册
    //GDALAllRegister();

    //// 设置中文路径
    //CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "No");

    //// 打开影像数据集
    //GDALDataset* inDataset;
    //inDataset = (GDALDataset*)GDALOpen(this->inFilePath.toStdString().c_str(), GA_ReadOnly);
    //if (inDataset == NULL)
    //{
    //    QMessageBox::warning(this, "Warning", "Invalid input file path");
    //    return;
    //}

    GDALDataset* poDataset;
    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");    // 设置GDAL支持中文路径
    CPLSetConfigOption("SHAPE_ENCODING", "");
    qDebug() << this->inFilePath.toStdString().c_str();
    poDataset = (GDALDataset*)GDALOpen(this->inFilePath.toStdString().c_str(), GA_ReadOnly);
    if (poDataset == NULL) {
        QMessageBox::warning(this, "Warning", "Invaild input file name.");
        return;
    }
    // 获取影像尺寸信息
    const int rowHeightY = poDataset->GetRasterYSize();;
    const int colWidthX = poDataset->GetRasterXSize();
    const int bandNum = poDataset->GetRasterCount();
    long bandSize = rowHeightY * colWidthX;
    // 读取影像像素数据到数组
    float* pInputData = new float[rowHeightY * colWidthX * bandNum];
    int* panBandMap = new int[bandNum];
    for (int i = 0; i < bandNum; ++i)
        panBandMap[i] = i + 1;
    if (poDataset->RasterIO(GF_Read, 0, 0, colWidthX, rowHeightY, \
        pInputData, colWidthX, rowHeightY, \
        GDT_Float64, bandNum, panBandMap, 0, 0, 0) != CE_None)
        return;
    // 关闭数据集
    GDALClose((GDALDatasetH)poDataset);

    // 影像像素数组存储行主序--列主序
    float* pProcessData = new float[rowHeightY * colWidthX * bandNum];
    for (int b = 0; b < bandNum; ++b)
        for (int i = 0; i < rowHeightY; ++i)
            for (int j = 0; j < colWidthX; ++j)
                pProcessData[b * bandSize + j * rowHeightY + i] = \
                pInputData[b * bandSize + i * colWidthX + j];


    // 将像素数组初始化为ammadillo中的多维矩阵数据结构
    fcube myFcube(pProcessData, rowHeightY, colWidthX, bandNum);
    // 输出部分像素值
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j)
            qDebug() << myFcube(i, j, 0) << " ";
        qDebug() << "\n";
    }

    // **************2-图像处理************************//
    // 对多维矩阵进行操作，将第一个波段所有像素置为0
    fcube resFcube = myFcube;
    resFcube.slice(0).zeros();

    // 获取多维矩阵的数据裸指针
    float* pResData = resFcube.memptr();

    // 影像像素数组存储列主序--行主序
    float* pOutputData = new float[rowHeightY * colWidthX * bandNum];
    for (int b = 0; b < bandNum; ++b)
        for (int i = 0; i < rowHeightY; ++i)
            for (int j = 0; j < colWidthX; ++j)
                pOutputData[b * bandSize + i * colWidthX + j] = \
                pResData[b * bandSize + j * rowHeightY + i];

    // **************3-写出影像************************//
    // 创建数据集
    const char* pszFormat = "GTiff";
    
    //驱动
    GDALDriver* poDriver;

    // 元信息
    char** papszMetadata;
    
    //输出数据集
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    GDALDataset* poDstDS;
    char** papszOptions = NULL;
    poDstDS = poDriver->Create(this->outFilePath.toStdString().c_str(), \
        colWidthX, rowHeightY, bandNum, GDT_Float32, papszOptions);
    // 写出数据
    if (poDstDS->RasterIO(GF_Write, 0, 0, colWidthX, rowHeightY, pOutputData, \
        colWidthX, rowHeightY, GDT_Float32, bandNum, \
        panBandMap, 0, 0, 0) != CE_None) {
        QMessageBox::warning(this, "Warning", "Write raster failed.");
        return;
    }
    // 关闭数据集
    GDALClose((GDALDatasetH)poDstDS);

    // 清理内存
    delete[] pInputData;
    delete[] panBandMap;
    delete[] pProcessData;
    delete[] pOutputData;

    // 处理提示
    QMessageBox::information(this, "Information", "Successfully!");
}
