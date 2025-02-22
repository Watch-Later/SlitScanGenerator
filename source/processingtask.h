#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H
#include <QString>
#include <memory>
#include <vector>
#include <QSettings>
#include "videoreader.h"
#include "imagewriter.h"
#include "configio.h"
#include "processingtaskreporter.h"
#include "cimg_tools.h"

struct ProcessingTask
{
    public:
        enum class Mode {
            XZ,
            ZY
        };

        enum class AngleMode {
            AngleNone=0,
            AngleRoll=1,
            AnglePitch=2,
        };

        enum class InterpolationMethod {
            NearestNeighbor=0,
            Linear=1,
            Cubic=2
        };

        static QString InterpolationMethod2String(InterpolationMethod m);
        static InterpolationMethod String2InterpolationMethod(const QString& m);
        interpolatingAtXYFunctor InterpolationMethod2XYFunctor(InterpolationMethod m);


        struct ProcessingItem {
        public:
            ProcessingItem();
            Mode mode;
            int location_x;
            int location_y;
            double angle;
            AngleMode angleMode;

            int angleModeForCombo() const;

            AngleMode filteredAngleMode() const;

            void save(std::shared_ptr<ConfigIO> ini, const std::string& basename) const;
            void load(std::shared_ptr<ConfigIO> ini, const std::string &basename) ;
        };

        ProcessingTask(std::shared_ptr<VideoReader> reader, std::shared_ptr<ImageWriter> writer, std::shared_ptr<ConfigIO> configio);

        ProcessingTask(ProcessingTask&&)=default;
        ProcessingTask& operator=(ProcessingTask&&)=default;

        ProcessingTask(const ProcessingTask&)=delete;
        ProcessingTask& operator=(const ProcessingTask&)=delete;

        void setReporter(ProcessingTaskReporter* reporter);

        /** \brief processes the task in the calling thread, use ProcessingThread for asynchronous processing in a QThread */
        void process();

        /** \brief switches off the saving of any data (images, ini, ...), usefull for dummy-processing or preview */
        bool do_not_save_anyting;

        QString filename;
        QString outputBasename;
        int outputFrames;
        QVector<ProcessingTask::ProcessingItem> pis;

        InterpolationMethod interpolationMethod;

        int stillCnt;
        int stillDelta;
        bool stillStrip;
        bool stillSeparateFiles;
        double stillGap;
        double stillBorder;
        double stillLineWidth;

        bool normalize;
        int normalizeX;
        int normalizeY;

        bool filterNotch;
        double fiterNotchWavelength;
        double fiterNotchWidth;

        bool modifyWhite;
        uint8_t whitepointR;
        uint8_t whitepointG;
        uint8_t whitepointB;

        void save(const QString& inifilename) const;
        void load(const QString& inifilename);

        bool processInit();
        bool processStep();
        void processFinalize();

        static void normalizeZY(cimg_library::CImg<uint8_t> &img, int normalizeY);
        static void normalizeXZ(cimg_library::CImg<uint8_t> &img, int normalizeX);
        static void applyFilterNotch(cimg_library::CImg<uint8_t> &img, double center, double delta, bool testoutput=false);
        static void applyWhitepointCorrection(cimg_library::CImg<uint8_t> &img, uint8_t red, uint8_t green, uint8_t blue, bool testoutput=false);
    private:
        struct ResultData {
            ResultData();
            cimg_library::CImg<uint8_t> img;
            int maxX;
            int maxY;
            int zs_val;
            QString filename;
            QString filt_filename;
            QString inifilename;
        };
        QVector<ResultData > results;
        int z;
        cimg_library::CImg<uint8_t> frame;
        QVector<cimg_library::CImg<uint8_t> > stillStripImg;
        bool m_saving;
        int m_savingFrame;
        int stills;

        void saveBase(std::shared_ptr<ConfigIO> ini) const;

        std::shared_ptr<VideoReader> m_reader;
        std::shared_ptr<ImageWriter> m_writer;
        std::shared_ptr<ConfigIO> m_configio;
        ProcessingTaskReporter* m_reporter;
        int m_prog;
};

#endif // PROCESSINGTASK_H
