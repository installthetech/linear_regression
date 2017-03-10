/*--  Inclusion of OpenCv header files  --*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/*--  Function declaration  --*/
int points();
void inverse(int x[],int y[]);
void B_xInv(float xInv[3][3],int y[]);
void linearfit(int x_array[],int y_array[],int in);

/*--  Variable declaration  --*/
float xInv[3][3];
int x_array[100],in;
int y_array[100];
Mat image(300,300,CV_32F,Scalar::all(0));/* Maximum value of data point is 300 */



/*--  Linear Curve or Line Fitting  --*/
void linearfit(int x_array[],int y_array[],int in)
{

        double sx = 0.0, sy = 0.0, stt = 0.0, sts = 0.0;
        int n = in;
        for (int i = 0; i < n; ++i)
        {
            sx += x_array[i];
            sy += y_array[i];
        }
        for (int i = 0; i < n; ++i)
        {
            double t = x_array[i] - sx/n;
            stt += t*t;
            sts += t*y_array[i];
        }

        double slope = sts/stt;
        double intercept = (sy - sx*slope)/n;

        for(int x=0;x<300;x++)/* Loop over all the values of 'x' i.e. 300 */
            {

                    float yy=(slope*x*1.0)+intercept;
                    cv::circle(image,cv::Point(x,yy),1,cv::Scalar(255,0,255),1);
            }

}

/*-- Inverse Matrix 'X' Multiplication with 'B' --*/
void B_xInv(float xInv[3][3],int y[])
{

    float inv[3][3];

    float BX[3];
    float temp=0;
    for(int i=0;i<3;i++)
    {
        temp=0;
        for(int j=0;j<3;j++)
        {
            temp+=(inv[i][j]*y[j]*1.0);
        }
        BX[i]=temp;
    }

    cout<<" \nThe solution matrix is "<<endl;
    cout<<BX[0]<<"  "<<BX[1]<<"  "<<BX[2]<<"  "<<endl;

    /*  Loop the equation over all the posssible values of 'x' */
    for(int x=0;x<300;x++)
    {

            float yy=(BX[0]*x*x)+(BX[1]*x)+BX[2];
            cv::circle(image,cv::Point(x,yy),1,cv::Scalar(255,0,0),1);
    }
     /* Calculation of deviation from the estimated line */
    for(int i=0;i<3;i++)
    {
            float yy=(BX[0]*x_array[i]*x_array[i])+(BX[1]*x_array[i])+BX[2];
            float diff=yy-y_array[i];
            cout<< "diff    "<<diff<<endl;
    }
}

/*--  Inverse Matrix Calculation  --*/
void inverse(int x_array[],int y_array[])
{
     int a[3][3];
     int i,j;
     float determinant=0;

     for(i=0;i<3;i++)
         {
          for(j=0;j<3;j++)
          {
              if(j==0)
                  a[i][j]=x_array[i]*x_array[i];
              if(j==1)
                  a[i][j]=x_array[i];
              if(j==2)
                  a[i][j]=1;
          }
         }


     for(i=0;i<3;i++)
     {
          determinant = determinant + (a[0][i]*(a[1][(i+1)%3]*
          a[2][(i+2)%3] - a[1][(i+2)%3]*a[2][(i+1)%3]));
     }
     if(determinant==0)
     {
         cout<<"Inverse does not exist (Determinant=0).\n";
     }
     else
     {
         cout<<"\nInverse of matrix is: \n";
     }
     for(i=0;i<3;i++)
     {
          for(j=0;j<3;j++)
          {
               cout<<((a[(i+1)%3][(j+1)%3] *a[(i+2)%3][(j+2)%3]) - (a[(i+1)%3][(j+2)%3]*a[(i+2)%3][(j+1)%3]))/ determinant<<"\t";
               xInv[i][j]=((a[(i+1)%3][(j+1)%3] *a[(i+2)%3][(j+2)%3]) - (a[(i+1)%3][(j+2)%3]*a[(i+2)%3][(j+1)%3]))*1.0/ determinant;
          }
      cout<<"\n";
     }

     B_xInv(xInv,y_array);

}

/*-- Data point collection from mouse click --*/
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
  if  ( event == EVENT_LBUTTONDOWN )
  {

      cv::circle(image,cv::Point(x,y),2,cv::Scalar(255,0,0),2);
      x_array[in]=x;
      y_array[in]=y;
      in++;

  }
    else if  ( event == EVENT_RBUTTONDOWN )
  {
        linearfit(x_array,y_array,in);
  }

}

/*-- Point Collection --*/
int points()
{
    while(true)
    {
        imshow("image",image);
        setMouseCallback("image", CallBackFunc, NULL);
            if(waitKey(1)>0)
            {
                break;
            }
    }
        return 0;
}

/*-- main() function --*/
int main(int argc, char ** argv)
{
    points();
    return 0;
}