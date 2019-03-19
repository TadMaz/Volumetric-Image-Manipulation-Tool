//VolImage Class
//Tadiwanashe Mazara
//MZRTAD001
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

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
            
            datafile.open("brain_mri_raws/"+baseName+".data");

            if(!datafile){
                cerr<<"Could not open file "<<baseName<<".data"<<endl;
                return -2;
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
                return -2;
            }

            cout<<width<<" "<<height<<" "<<no_of_slices<<endl;

            //reserve memory for vector containing slices
            slices.reserve(no_of_slices);

            //file stream for each slice
            ifstream imagefile;
            
            //for each slice file
            for (int i = 0; i < no_of_slices; i++){
                
                string filename  = "brain_mri_raws/"+baseName+std::to_string(i)+".raw";
                
                //try to open the image "slice".
                imagefile.open(filename);

                if(!imagefile){
                    cerr<<"Could not open image file "<<baseName+std::to_string(i)<<".raw"<<endl;
                    return -2;
                }

                //dyanamically allocate memory

                unsigned char ** slice = new unsigned char*[height];
                unsigned char pixel_value;

                cout<<"Starting memory allocation for slice "<<i<<endl;

                for(int rowNo = 0 ; rowNo < height; rowNo++){

                    slice[rowNo] = new u_char[width];

                    for (int colNo = 0; colNo < width; colNo++){
                        
                        pixel_value = imagefile.get();
                        slice[rowNo][colNo] = pixel_value;
                    }

                }
                slices.push_back(slice);
                
                cout<<"Dynamically allocated memory for slice "<<slices.size()<<endl;

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
            string filename = output_prefix+".raw";
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

             //initialise file output stream
            ofstream outfile;

            //open output file
            string filename =  output_prefix + raw_ext;
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
            
            //total bytes = (no of pixels * 1 byte each) + ( height* 8 bytes (64-bit architecture) ) 
            int total_bytes = (width*height) + height*8; 
            
            return total_bytes;
        }


    };    
}

int main(void ){

    MZRTAD001::VolImage image;

    image.readImages("MRI");
    image.diffmap(2,4,"difftest");
    image.extract(5,"extractest");

    return 0;
}