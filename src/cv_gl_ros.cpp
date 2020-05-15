//OpenGL_ROS. Slightly modified code from the OpenCV documentation that draws a
//cube every frame; this modified version uses the global variables rotx and roty that are
//connected to the sliders in Figure 9-6

// Note: This example needs OpenGL installed on your system. It doesn't build if 
//       the OpenGL libraries cannot be found.
#include <GL/gl.h>
#include <GL/glu.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/opengl.hpp>
#include <iostream>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/CameraInfo.h>

#define RADPERDEG 0.0174533

using namespace std;

int rotx = 55, roty = 40;

cv::Mat texture;


//Class to acquire images and camera information from ROS topic
class ImageROS
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;

    ros::Subscriber camera_info_sub;

    cv::Mat rxd_img;
	
public:
    ImageROS()
    	: it_(nh_)
    {
    	image_sub_ = it_.subscribe("/usb_cam/image_raw",1,&ImageROS::imageCb, this);
    	
        camera_info_sub = nh_.subscribe("/usb_cam/camera_info",1,&ImageROS::cameraInfoCb,this);
        image_pub_ = it_.advertise("/opengl_img",1);

    	
    }
    ~ImageROS()
    {

    }
    void imageCb(const sensor_msgs::ImageConstPtr& msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        namespace enc = sensor_msgs::image_encodings;

        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
            rxd_img = cv_ptr->image;

        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception %s",e.what());
            return;
        }
        

    }

    void cameraInfoCb(const sensor_msgs::CameraInfoConstPtr& info_msg)
    {
       ;

    }
    cv::Mat getImage(void)
    {
        cv::Mat test_img;
        if(!rxd_img.empty())
        {
            return rxd_img;
        }
        else
        {
            return test_img;
        }
    }
    //TODO
    void publishGLImage(cv::Mat gl_img)
    {
        if(!gl_img.empty())
        {

            cv_bridge::CvImage out_msg;
            //out_msg.header   = in_msg->header; // Same timestamp and tf frame as input image
            out_msg.encoding = sensor_msgs::image_encodings::BGR8; // Or whatever
            out_msg.image    = gl_img; // Your cv::Mat

            image_pub_.publish(out_msg.toImageMsg());



        }
    }
    
 

};


void on_opengl(void* param) {

    cv::ogl::Texture2D* backgroundTex = (cv::ogl::Texture2D*)param;
    glEnable( GL_TEXTURE_2D );
    backgroundTex->bind();
    cv::ogl::render(*backgroundTex);
    glDisable( GL_TEXTURE_2D );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef(0, 0, -0.5);
    glRotatef( rotx, 1, 0, 0 );
    glRotatef( roty, 0, 1, 0 );
    glRotatef( 0, 0, 0, 1 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    static const int coords[6][4][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };
    for (int i = 0; i < 6; ++i) {
        glColor3ub( i*20, 100+i*10, i*42 );
        glBegin( GL_QUADS );
        for (int j = 0; j < 4; ++j) {
            glVertex3d(
                        0.3 * coords[i][j][0],
                    0.3 * coords[i][j][1],
                    0.3 * coords[i][j][2]
                    );
        }
        glEnd();
    
}
      
    
}

void on_trackbar( int, void* ) {
    cv::updateWindow( "OpenGL_ROS" );
}

void help(char ** argv) {
	
	cout << "\n//OpenGL_ROS. Slightly modified code from the OpenCV documentation that draws a"
		<< "\n//cube every frame; this modified version uses the global variables rotx and roty that are"
		<< "\n//connected to the sliders in Figure 9-6"
		<< "\n// Note: This example needs OpenGL installed on your system. It doesn't build if" 
		<< "\n//       the OpenGL libraries cannot be found.\n\/"
		<< "\nCall: " << argv[0] << " <image>\n\n"
		<< "\nHere OpenGL is used to render a cube on top of an image.\n"
        << "\nUser can rotate the cube with the sliders\n" <<endl;
}

int main(int argc, char* argv[])
{

    ros::init(argc,argv,"opengl_node");
    
    ImageROS obj;
    cv::Mat img;     

    ros::Rate r(10); // 10 hz


    while(img.empty())
    {
        img = obj.getImage();
        ros::spinOnce();
        ROS_INFO("Waiting for images");
        ros::Duration(1.0).sleep(); 
    }
 
     if (img.empty()) {
        cout << "Cannot load image: " << endl;
        exit(EXIT_FAILURE);
    }    


    cv::namedWindow( "OpenGL_ROS", CV_WINDOW_OPENGL );
    cv::resizeWindow("OpenGL_ROS", img.cols, img.rows);
    cv::createTrackbar( "X-rotation", "OpenGL_ROS", &rotx, 360, on_trackbar);
    cv::createTrackbar( "Y-rotation", "OpenGL_ROS", &roty, 360, on_trackbar);

    
   while(ros::ok())
   { 
   	img = obj.getImage();
   	cv::ogl::Texture2D backgroundTex(img,true);   
   	cv::setOpenGlDrawCallback("OpenGL_ROS", on_opengl, &backgroundTex);
   	cv::updateWindow("OpenGL_ROS");
        ros::spinOnce();   
        int key = cv::waitKey(30);	

        if (key == 27) break; // if escape pressed then break
	backgroundTex.release(); //Releasing the background texture to avoid memory leak
	

   }
   cv::setOpenGlDrawCallback("OpenGL_ROS", 0, 0);
   cv::destroyAllWindows();

    return 0;
}
