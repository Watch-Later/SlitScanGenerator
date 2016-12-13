#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H
#include <QString>
#include <vector>
#include "ffmpeg_tools.h"
#include "cimg_tools.h"

struct ProcessingTask
{
    public:
        enum class Mode {
            XZ,
            ZY
        };

        struct ProcessingItem {
            inline ProcessingItem(): mode(ProcessingTask::Mode::XZ), location(-1) {}
            Mode mode;
            int location;
        };

        ProcessingTask();

        QString filename;
        int outputFrames;
        QVector<ProcessingTask::ProcessingItem> pis;

        int stillCnt;
        int stillDelta;
        bool stillStrip;
        bool stillSeparateFiles;

        bool normalize;
        int normalizeX;
        int normalizeY;

        bool filterNotch;
        double fiterNotchWavelength;
        double fiterNotchWidth;


        bool processInit(int &prog, int &maxProg, QString &message, QString &error);
        bool processStep(int& prog, int& maxProg, QString &message);
        void processFinalize();

        static void normalizeZY(cimg_library::CImg<uint8_t> &img, int normalizeY);
        static void normalizeXZ(cimg_library::CImg<uint8_t> &img, int normalizeX);
        static void applyFilterNotch(cimg_library::CImg<uint8_t> &img, double center, double delta, bool testoutput=false);
    private:
        QVector<cimg_library::CImg<uint8_t> > results;
        QVector<QString> result_filenames;
        QVector<QString> resultfilt_filenames;
        QVector<QString> result_inifilenames;
        FFMPEGVideo* vid;
        int z;
        cimg_library::CImg<uint8_t> frame;
        QVector<cimg_library::CImg<uint8_t> > stillStripImg;
        bool m_saving;
        int m_savingFrame;
        int stills;

};

#endif // PROCESSINGTASK_H
