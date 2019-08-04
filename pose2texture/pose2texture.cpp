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



static struct option long_options[] = {
    {"verbose",  		required_argument, 0, 'v'},
    {"help",     		no_argument,       0, 'h'},
    {"vui-folder",  	required_argument, 0, 'i'},
    {"sum",     		no_argument,       0, 's'},
    {"max",     		no_argument,       0, 'm'},
    {"density",     	no_argument,       0, 'd'},
    {"white",     	no_argument,       0, 'w'},
    {0, 0, 0, 0}
};


void shift(Mat textmp,Mat texture,bool white){
int row,col;
        
for(int row=0; row<600-1; row++)
    for(int col=0; col<400; col++) {
        Vec4f   texsrc=texture.at<Vec4f>(row,col);
        Vec4f   tmpsrc=texture.at<Vec4f>(row+1,col);
        if(texsrc[3]&&!tmpsrc[3]){
            if(white && (texsrc[0]>190)&&(texsrc[1]>190)&&(texsrc[2]>190)) // skip white
                continue;
            //texsrc[3]+=(float)1.0;    
            textmp.at<Vec4f>(row+1,col)+=texsrc;
        }
    }
for(int row=0; row<600; row++)
    for(int col=0; col<400-1; col++) {
        Vec4f texsrc=texture.at<Vec4f>(row,col);
        Vec4f tmpsrc=texture.at<Vec4f>(row,col+1);
        if(texsrc[3]&&!tmpsrc[3]){
            if(white && (texsrc[0]>190)&&(texsrc[1]>190)&&(texsrc[2]>190)) // skip white
                                continue;
            //texsrc[3]+=(float)1.0;    
            textmp.at<Vec4f>(row,col+1)+=texsrc;
        }
    }
for(int row=0; row<600-1; row++)
    for(int col=0; col<400-1; col++) {
        Vec4f texsrc=texture.at<Vec4f>(row,col);
        Vec4f tmpsrc=texture.at<Vec4f>(row+1,col+1);
        if(texsrc[3]&&!tmpsrc[3]){
            if(white && (texsrc[0]>190)&&(texsrc[1]>190)&&(texsrc[2]>190)) // skip white
                                continue;
            //texsrc[3]+=(float)1.0;    
            textmp.at<Vec4f>(row+1,col+1)+=texsrc;
        }
    }
    
for(int row=0+1; row<600; row++)
    for(int col=0; col<400; col++) {
        Vec4f texsrc=texture.at<Vec4f>(row,col);
        Vec4f tmpsrc=texture.at<Vec4f>(row-1,col);
        if(texsrc[3]&&!tmpsrc[3]){
                        if(white && (texsrc[0]>190)&&(texsrc[1]>190)&&(texsrc[2]>190)) // skip white
                                continue;
            //texsrc[3]+=(float)1.0;    
            textmp.at<Vec4f>(row-1,col)+=texsrc;
        }
    }    
for(int row=0; row<600; row++)
    for(int col=0+1; col<400; col++) {
        Vec4f texsrc=texture.at<Vec4f>(row,col);
        Vec4f tmpsrc=texture.at<Vec4f>(row,col-1);
        if(texsrc[3]&&!tmpsrc[3]){
                        if(white && (texsrc[0]>190)&&(texsrc[1]>190)&&(texsrc[2]>190)) // skip white
                                continue;
            //texsrc[3]+=(float)1.0;    
            textmp.at<Vec4f>(row,col-1)+=texsrc;
        }
    }   
 for(int row=0+1; row<600; row++)
    for(int col=0+1; col<400; col++) {
        Vec4f texsrc=texture.at<Vec4f>(row,col);
        Vec4f tmpsrc=texture.at<Vec4f>(row-1,col-1);
        if(texsrc[3]&&!tmpsrc[3]){
                        if(white && (texsrc[0]>190)&&(texsrc[1]>190)&&(texsrc[2]>190)) // skip white
                                continue;
            //texsrc[3]+=(float)1.0;    
            textmp.at<Vec4f>(row-1,col-1)+=texsrc;
        }
    }              
            
}

int main(int argc, char** argv )
{
    boost::filesystem::path iuvpath=boost::filesystem::path("./");
    bool    do_sum=false;
    bool    do_max=false;
    bool    do_density=false;
    bool    white=false;
    
    int 	option_index = 0;
    while(1) {
        int c = getopt_long (argc, argv, "hv:i:smw",long_options, &option_index);
        if (c == -1)
            break;  // end of options
        switch (c) {
        case 'v':
            //verbose_level=atoi(optarg);
            break;
        case 'h':
            //usage(cerr,basename(argv[0]));
            white=true; // white removal
            break;		// never reached
        case 'w':
            //usage(cerr,basename(argv[0]));
            //exit(1);
            break;		// never reached
        case 'i':
            // path to _INDS.png and _IUV.png folder
            iuvpath=boost::filesystem::path(optarg);
            break;
        }
    }
    cout << "iuv-folder: " << iuvpath << endl;
    
    Mat   texsum(Size(400,600),CV_32FC4,Scalar(0,0,0,0));
    Mat   texmax(Size(400,600),CV_32FC4,Scalar(0,0,0,0));
    
    int count=0;

    for(int i=optind; i<argc; i++) {
        boost::filesystem::path bgrimpath(argv[i]);
        boost::filesystem::path bgrimbasename=basename(bgrimpath);
        
        // skip *_INDS.png, *._IUV.png
        if(string::npos!=bgrimbasename.string().find("_INDS")) {
            cerr << basename(argv[0]) << ": skipping _INDS file  '" << argv[i] << "'" << endl;
            continue;
        }
        if(string::npos!=bgrimbasename.string().find("_IUV")) {
            cerr << basename(argv[0]) << ": skipping _IUV file  '" << argv[i] << "'" << endl;
            continue;
        }
        if(string::npos!=bgrimbasename.string().find("_DEN")) {
            cerr << basename(argv[0]) << ": skipping _DEN file  '" << argv[i] << "'" << endl;
            continue;
        }
        if(string::npos!=bgrimbasename.string().find("_TEX")) {
            cerr << basename(argv[0]) << ": skipping _TEX file  '" << argv[i] << "'" << endl;
            continue;
        }

        // read the source image file
        Mat imgsrc=imread(argv[i],1);
        if(imgsrc.empty()) {
            cerr << basename(argv[0]) << ": could not open file or invalid image file  '" << argv[i] << "'" << endl;
            continue;
        }
        if(imgsrc.channels()!=3) {
            cerr << basename(argv[0]) << ": image file is not RGB '" << argv[i] << "'" << endl;
            continue;
        }

        Size srcsize=imgsrc.size();
        cout << "processing: " << string(argv[i]) << endl;

        // get IUV file
        boost::filesystem::path vuiimpath= iuvpath / boost::filesystem::path(bgrimbasename.string() + string("_IUV.png"));
        cout << "\t- " << vuiimpath << endl;

        Mat vuisrc=imread(vuiimpath.c_str(),1);
        if(vuisrc.empty()) {
            cerr << basename(argv[0]) << ": could not open file or invalid _IUV file  '" << vuiimpath << "'" << endl;
            continue;
        }
        if(vuisrc.channels()!=3) {
            cerr << basename(argv[0]) << ": _IUV file is not RGB '" << vuiimpath << "'" << endl;
            continue;
        }
        Size vuisize=vuisrc.size();


        // get INDS file
        boost::filesystem::path indimpath= iuvpath / boost::filesystem::path(bgrimbasename.string() + string("_INDS.png"));
        cout << "\t- " << indimpath << endl;
#if 0
        Mat indsrc=imread(indimpath.c_str(),-1);
        if(indsrc.empty()) {
            cerr << basename(argv[0]) << ": could not open file or invalid _INDS file  '" << indimpath << "'" << endl;
        } else if(indsrc.channels()!=1) {
            cerr << basename(argv[0]) << ": _INDS file is not 1 channel'" << indimpath << "'" << endl;
        } else {
            Size indsize=indsrc.size();

            // get number of different values in ind matrix => should be nb of people seen
            std::vector<unsigned char> unik = unique(indsrc, true);

            for (unsigned int i = 0; i < unik.size(); i++)
                std::cout << (short)unik[i] << " ";
        }
#endif

        boost::filesystem::path teximpath= boost::filesystem::path(bgrimbasename.string() + string("_TEX.png"));
        cout << "\t-> " << teximpath << endl;
        boost::filesystem::path denimpath= boost::filesystem::path(bgrimbasename.string() + string("_DEN.png"));
        cout << "\t-> " << denimpath << endl;

        // texture image
        Mat   texture(Size(400,600),CV_32FC4,Scalar(0,0,0,0));
        
        // create tile headers on the texture parts
        // 4x6=24 tiles, tile size 200*200
        vector<Mat>  tiles;
        for(int row=0; row<4; row++)
            for(int col=0; col<6; col++)
                tiles.push_back(texture(Rect(100*row,100*col,100,100)));

        // project original photo on tiles (i.e. texture image)
        // and keep count of pixel access on layer 3
        for(int row=0; row<vuisize.height; row++)
            for(int col=0; col<vuisize.width; col++) {
                unsigned int ind=0;
                Vec3b        vui=vuisrc.at<Vec3b>(row,col);

                int i= (unsigned)0x1F & vui[0];
                int u=(255.-float(vui[2]))*199./510.;
                int v=float(vui[1])*199./510.;

                if((i>0)&&(i<25))
                    if((u>=0)&&(u<100))
                        if((v>=0)&&(v<100)) {
                            Vec3b pixsrc=imgsrc.at<Vec3b>(row,col); // original photo pixel
                            if(white && (pixsrc[0]>190)&&(pixsrc[1]>190)&&(pixsrc[2]>190)) // skip white
                                continue;
                                
                            Vec4f texsrc=tiles[i-1].at<Vec4f>(u,v); // texture position
                            texsrc[0]+=(float)pixsrc[0];
                            texsrc[1]+=(float)pixsrc[1];
                            texsrc[2]+=(float)pixsrc[2];
                            texsrc[3]+=(float)1.0;                  // one more access
                            tiles[i-1].at<Vec4f>(u,v)=texsrc;
                        }
            }
        // => texture(b,g,r,access count)
        // => density matrix = access count
        
        const int ksize=7;
        
        Mat   density;
        Mat   texdensity;
        {
        vector<Mat>   v;
        split(texture,v);
        density=v[3];
        GaussianBlur(v[3],texdensity,Size(ksize,ksize),0,0);
        if(do_density)
            imwrite(denimpath.c_str(),density);
        }
        // max texture = keep the pixel values with maximum access 

        Mat   maxdensity;
        {
        vector<Mat>   v;    
        split(texmax,v);
        GaussianBlur(v[3],maxdensity,Size(ksize,ksize),0,0);
        }
        
        for(int row=0; row<600; row++)
            for(int col=0; col<400; col++) {
                Vec4f texsrc=texture.at<Vec4f>(row,col);
                Vec4f maxsrc=texmax.at<Vec4f>(row,col);
                float texden=texdensity.at<float>(row,col);
                float maxden=maxdensity.at<float>(row,col);          
                if(maxden<texden)
                    texmax.at<Vec4f>(row,col)=texsrc;
            }
            
        /*
        // max texture = keep the pixel values with maximum access 
        for(int row=0; row<1200; row++)
            for(int col=0; col<800; col++) {
                Vec4f texsrc=texture.at<Vec4f>(row,col);
                Vec4f maxsrc=texmax.at<Vec4f>(row,col);
                
                if(maxsrc[3]<texsrc[3])
                    texmax.at<Vec4f>(row,col)=texsrc;
            }
        */
            
            
        // => accumulate textures (Sb,Sg,Sr,S access count)
        texsum=texsum+texture;
        
        count+=1;
        
        Mat   textmp(Size(400,600),CV_32FC4,Scalar(0,0,0,0));  
        
        shift(textmp,texture,white);
        shift(textmp,textmp.clone(),white);
        shift(textmp,textmp.clone(),white);
        textmp+=texture;
        textmp+=texture;
        textmp+=texture;
        textmp+=texture;
        textmp+=texture;
        textmp+=texture;
        textmp+=texture;
        textmp+=texture;    
        // the unshifted one
        for(int row=0; row<600; row++)
            for(int col=0; col<400; col++) {
                Vec4f texsrc=textmp.at<Vec4f>(row,col);
                if(texsrc[3]){
                    texsrc[0]=float(texsrc[0])/float(texsrc[3]);
                    texsrc[1]=float(texsrc[1])/float(texsrc[3]);
                    texsrc[2]=float(texsrc[2])/float(texsrc[3]);
                    texsrc[3]=(float)255.0;
                    textmp.at<Vec4f>(row,col)=texsrc;
                }
            }
        
        texture=textmp;
        imwrite(teximpath.c_str(),texture);
        cout << endl;
    }

    
    for(int row=0; row<600; row++)
        for(int col=0; col<400; col++) {
            Vec4f texsrc=texsum.at<Vec4f>(row,col);
            texsrc[0]=float(texsrc[0])/float(texsrc[3]);
            texsrc[1]=float(texsrc[1])/float(texsrc[3]);
            texsrc[2]=float(texsrc[2])/float(texsrc[3]);
            //texsrc[3]=(float)255.0;
            texsrc[3]=(float)(texsrc[3]>0.0 ? 255.0:0.0);
            texsum.at<Vec4f>(row,col)=texsrc;
        }
    if(do_sum)
    imwrite("sum_TEX.png",texsum);
    
    for(int row=0; row<600; row++)
        for(int col=0; col<400; col++) {
            Vec4f texsrc=texmax.at<Vec4f>(row,col);
            texsrc[0]=float(texsrc[0])/float(texsrc[3]);
            texsrc[1]=float(texsrc[1])/float(texsrc[3]);
            texsrc[2]=float(texsrc[2])/float(texsrc[3]);
            //texsrc[3]=(float)255.0;
            texsrc[3]=(float)(texsrc[3]>0.0 ? 255.0:0.0);
            texmax.at<Vec4f>(row,col)=texsrc;
        }
    if(do_max)
    imwrite("max_TEX.png",texmax);
    
    return 0;
}
