#include <getopt.h>
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include <algorithm>
#include <vector>
using namespace std;
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace cv;

//https://stackoverflow.com/questions/24716932/obtaining-list-of-unique-pixel-values-in-opencv-mat
//
std::vector<unsigned char> unique(const cv::Mat& input, bool sort = false)
{
    if (input.channels() > 1 || input.type() != CV_8UC1) {
        std::cerr << "unique !!! Only works with CV_8U 1-channel Mat" << std::endl;
        return std::vector<unsigned char>();
    }

    std::vector<unsigned char> out;
    for (int y = 0; y < input.rows; ++y) {
        const unsigned char* row_ptr = input.ptr<unsigned char>(y);
        for (int x = 0; x < input.cols; ++x) {
            unsigned char value = row_ptr[x];

            if ( std::find(out.begin(), out.end(), value) == out.end() )
                out.push_back(value);
        }
    }

    if (sort)
        std::sort(out.begin(), out.end());

    return out;
}

static struct option long_options[] = {
    {"verbose",  		required_argument, 0, 'v'},
    {"help",     		no_argument,       0, 'h'},
    {"text-folder",  	required_argument, 0, 'i'},
    {0, 0, 0, 0}
};

int main(int argc, char** argv )
{
    boost::filesystem::path iuvpath=boost::filesystem::path("default_IUV.png");

    int 	option_index = 0;
    while(1) {
        int c = getopt_long (argc, argv, "hv:i:",long_options, &option_index);
        if (c == -1)
            break;  // end of options
        switch (c) {
        case 'v':
            //verbose_level=atoi(optarg);
            break;
        case 'h':
            //usage(cerr,basename(argv[0]));
            //exit(1);
            break;		// never reached
        case 'i':
            // path to _INDS.png and _IUV.png folder
            iuvpath=boost::filesystem::path(optarg);
            break;
        }
    }

    // read the source iuv file
    Mat vuisrc=imread(iuvpath.c_str(),1);
    if(vuisrc.empty()) {
        cerr << basename(argv[0]) << ": could not open file or invalid iuv file  " << iuvpath << " " << endl;
        exit(-1);
    }
    if(vuisrc.channels()!=3) {
        cerr << basename(argv[0]) << ": image file is not RGB " << iuvpath << " " << endl;
        exit(-1);
    }
    Size iuvsize=vuisrc.size();
        
    // for all texture files
    for(int i=optind; i<argc; i++) {
        boost::filesystem::path txtimpath(argv[i]);
        boost::filesystem::path txtimbasename=basename(txtimpath);
        // skip any file not _TEX
        if(string::npos==txtimbasename.string().find("_TEX")) 
            continue;
        
        cout << "processing: " << string(argv[i]) << endl;
        Mat   texture=imread(argv[i],-1);
         if(texture.empty()) {
            cerr << basename(argv[0]) << ": could not open file or invalid _TEX file  '" << txtimpath << "'" << endl;
            continue;
        }
        if(texture.channels()!=4) {
            cerr << basename(argv[0]) << ": _TEX file is not RGBA '" << txtimpath << "'" << endl;
            continue;
        }
        // create tile headers on the texture parts
        // 4x6=24 tiles, tile size 200*200
        vector<Mat>  tiles;
        for(int row=0; row<4; row++)
            for(int col=0; col<6; col++)
                tiles.push_back(texture(Rect(200*row,200*col,200,200)));
         
        // project original photo on tiles (i.e. texture image)
        // and keep count of pixel access on layer 3
        Mat imgsrc(vuisrc.size(),CV_8UC4,Scalar(0,0,0,0));
        for(int row=0; row<iuvsize.height; row++)
            for(int col=0; col<iuvsize.width; col++) {
                unsigned int ind=0;
                Vec3b        vui=vuisrc.at<Vec3b>(row,col);

                int i= (unsigned)0x1F & vui[0];
                int u=(255.-float(vui[2]))*199./255.;
                int v=float(vui[1])*199./255.;

                if((i>0)&&(i<25))
                    if((u>=0)&&(u<200))
                        if((v>=0)&&(v<200)) {
                            imgsrc.at<Vec4b>(row,col)=tiles[i-1].at<Vec4b>(u,v);
                        }
            }
        imshow("toto",imgsrc);
        waitKey(0);
    }
    return 0;
}
