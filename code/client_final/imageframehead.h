#ifndef IMAGEFRAMEHEAD_H
#define IMAGEFRAMEHEAD_H

struct ImageFrameHead {
    //功能码24
    int funCode;                        //!功能码
    //表头大小：28
    unsigned int uTransFrameHdrSize;    //!sizeof(WIFI_FRAME_HEADER)
    //包数据大小：996，最后一个包不一定
    unsigned int uTransFrameSize;       //!sizeof(WIFI_FRAME_HEADER) + Data Size

    //数据帧变量
    //文件总大小
    unsigned int uDataFrameSize;        //数据帧的总大小
    //包的个数
    unsigned int uDataFrameTotal;       //一帧数据被分成传输帧的个数
    //包的序号
    unsigned int uDataFrameCurr;        //数据帧当前的帧号
    //数据996的整数倍
    unsigned int uDataInFrameOffset;    //数据帧在整帧的偏移
};
#endif // IMAGEFRAMEHEAD_H
