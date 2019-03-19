//VolImage Class
//Tadiwanashe Mazara
//MZRTAD001
//March 2019 

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

#define raw_ext ".raw"
#define header_ext ".data"

using namespace std;

namespace MZRTAD001
{   
    class VolImage
    {
    private:
        int width, height; // width and height of image stack
        std::vector<unsigned char**> slices; // data for each slice, in order

    public:
        // default constructor - define in .cpp
        VolImage()
        {
            width = 0;
            height = 0;
            slices = {};
        } 
         // destructor - define in .cpp file
        ~VolImage(){

            //clear array containing pointers
            for (int i = 0; i < slices.size(); i++){
                
                //clear array containing values
                for(int j = 0; j < height; j++){
                    delete [] slices[i][j];  
                }
                delete[] slices[i];
            }
        }
       
         // populate the object with images in stack and
        bool readImages(std::string baseName){

            int no_of_slices = 0;

            //open .dat file in subfolder and read width, height and number of images
            ifstream datafile;

            string filename = baseName+"/"+baseName+header_ext;
            
            datafile.open(filename);

            if(!datafile){
                cerr<<"Could not open file "<<baseName<<header_ext<<endl;
                return false;
            }

            //get line with header info and extract numbers

            string header;
            std::getline(datafile, header);
            istringstream iss(header);
            
            //set dimension values

            iss>>width;
            iss>>height;
            iss>>no_of_slices;

            //close file
            datafile.close();
            
            //check if header has been read correctly.

            if (width ==0 ||height ==0||no_of_slices ==0){
                cerr<<"Header file could not be read."<<endl;
                return false;
            }

            //reserve memory for vector containing slices
            slices.reserve(no_of_slices);

            //file stream for each slice
            ifstream imagefile;
            
            //for each slice file
            for (int i = 0; i < no_of_slices; i++){
                
                filename  = baseName+"/"+baseName+std::to_string(i)+raw_ext;
                
                //try to open the image "slice".
                imagefile.open(filename);

                if(!imagefile){
                    cerr<<"Could not open image file "<<baseName+std::to_string(i)<<raw_ext<<endl;
                    return false;
                }

                //dyanamically allocate memory

                unsigned char ** slice = new unsigned char*[height];
                unsigned char pixel_value;

                for(int rowNo = 0 ; rowNo < height; rowNo++){

                    slice[rowNo] = new u_char[width];

                    for (int colNo = 0; colNo < width; colNo++){
                        
                        pixel_value = imagefile.get();
                        slice[rowNo][colNo] = pixel_value;
                    }

                }
                slices.push_back(slice);

                //close the image "slice".
                imagefile.close();
            }
        }


        // compute difference map and write out; define in .cpp
        
        void diffmap(int sliceI, int sliceJ, std::string output_prefix){
            
            //print function call
            cout<<"Difference Map Function called."<<endl;
            cout<<slices.size()<<endl;
            //initialise file output stream
            ofstream outfile;

            //open output file
            string filename = output_prefix+raw_ext;
            outfile.open(filename);

            if(!outfile){
                cerr<<"Failed to open file : "<<filename<<" to write difference map"<<endl;
                return;
            }

            //write diff to output file.
            unsigned char diff;

            for (int r = 0; r < height; r++){

                for(int c = 0; c < width; c++){
                   
                   diff =  (unsigned char)(abs((float)slices.at(sliceI)[r][c] - (float)slices.at(sliceJ)[r][c])/2);
                   outfile<<diff;
                   
                }
            }    
            
            //close file
            outfile.close();

            return;
        }
        
        // extract slice sliceId and write to output - define in .cpp
        void extract(int sliceId, std::string output_prefix){

            //print function call
            cout<<"Extract function called"<<endl;

            if (!slices.size()){
                cout<<slices.size()<<endl;
                cerr<<"The loaded image is empty. No slice to extract."<<endl;
                return;
            }

            if(sliceId >=slices.size()){
                cerr<<"Slice ID given is out of range.Only "<<slices.size()<<" slices loaded."<<endl;
                return;
            }

            //1. initialise data file output stream
            ofstream datafile;

            //open output file
            string filename =  output_prefix + header_ext;
            datafile.open(filename);
            
            if(!datafile){
                cerr<<"Failed to open file : "<<filename<<" while extracting."<<endl;
                return;
            }

            //write to data file
            datafile<<width<<" "<<height<<" "<<1;

            //close file
            datafile.close();

            //2. initialise file output stream
            ofstream outfile;

            //open output file
            filename =  output_prefix + raw_ext;
            outfile.open(filename);
            
            if(!outfile){
                cerr<<"Failed to open file : "<<filename<<" to write extract slice"<<endl;
                return;
            }

            //write extacted pixels to file
            
            for (int r = 0; r < height; r++){

                for(int c = 0; c < width; c++){

                    outfile<<(unsigned char)slices[sliceId][r][c];       
                }
            }

            //close file
            outfile.close();
            
            return;
        }

        // number of bytes uses to store image data bytes
        //and pointers (ignore vector<> container, dims etc)
        int volImageSize(void){
            
            //total bytes = slices x (no of pixels x 1 byte each) + ( (height+1 ) x 8 bytes (64-bit architecture) ) 
            int total_bytes = ( (width*height) + (height+1)*8 )* getNumberOfSlices(); 
            
            return total_bytes;
        }

        //return number of slices "images" in volumetric image
        int getNumberOfSlices(void){

            return slices.size();
        }

        /* extracts a row from all slices in the image and creates a new image"*/
        void extractRowAcrossSlices(int rowNo){

             //print function call
            cout<<"ExtractRowAcrossSlices function called"<<endl;

            if (!slices.size()){
                cout<<slices.size()<<endl;
                cerr<<"The loaded image is empty. No slice to extract."<<endl;
                return;
            }

            if(rowNo >= height){
                cerr<<"Row number given is out of range.Only "<<slices.size()<<" slices loaded."<<endl;
                return;
            }

            //1. initialise data file output stream
            ofstream datafile;

            //open output file
            string filename =  string("output") + header_ext;
            datafile.open(filename);
            
            if(!datafile){
                cerr<<"Failed to open file : "<<filename<<" while extracting."<<endl;
                return;
            }

            //write to data file
            datafile<<width<<" "<<getNumberOfSlices()<<" "<<1;

            //close file
            datafile.close();

            //2. initialise file output stream
            ofstream outfile;

            //open output file
            filename =  string("output") + raw_ext;
            outfile.open(filename);
            
            if(!outfile){
                cerr<<"Failed to open file : "<<filename<<" to write extract slice"<<endl;
                return;
            }

            //write extacted pixels to file
            
            for (int i = 0; i < getNumberOfSlices(); i++){

                for(int c = 0; c < width; c++){

                    outfile<<(unsigned char)slices[i][rowNo][c];       
                }
            }

            //close file
            outfile.close();
            
            return;
        }

    };    
}

int main(int argv , char*argc[] ){

    //instantiate Volumentric Image object
    MZRTAD001::VolImage image;

    string base_name;

    //1 argument, run readImages + display 
    if (argv == 2){
        
        base_name = argc[1];
        image.readImages(base_name);
        
        cout<<"Number of images: "<<image.getNumberOfSlices()<<endl;
        cout<<"Number of bytes required: "<<image.volImageSize()<<endl;
    
    //Differential Map 
    }else if( strcmp(argc[2] , "-d") == 0 && argv == 6){
        
        base_name = argc[1];

        //format sliceI and sliceJ
        int sliceI;
        int sliceJ;

        stringstream convertI(argc[3]);
        convertI>>sliceI;
        
        stringstream convertJ(argc[4]);
        convertJ>>sliceJ;
        
        //output file
        string outfile = argc[5];

        //read images
        image.readImages(base_name);
        
        //difference map
        image.diffmap(sliceI,sliceJ,outfile);
    
    //Extraction
    }else if (strcmp(argc[2] , "-x") == 0 && argv == 5 ){

        base_name = argc[1];

        //format sliceI
        int sliceI;

        stringstream convertI(argc[3]);
        convertI>>sliceI;

        //output file
        string outfile = argc[4];

        //read images
        image.readImages(base_name);

        //extraction
        image.extract(sliceI,outfile);
    
    }else if (strcmp(argc[2] , "-g") == 0 && argv == 4){

        base_name = argc[1];

        //format sliceI
        int rowI;

        stringstream convertI(argc[3]);
        convertI>>rowI;

        //read images
        image.readImages(base_name);

        //extraction
        image.extractRowAcrossSlices(rowI);
    
    }else{

        cerr<<"Invalid arguments. Arguments should be in form :"<<endl;
        cerr<<"volimage <imageBase> [-d i j output_file_name] [-x i output_file_name]"<<endl;
    }
 
    return 0;
}