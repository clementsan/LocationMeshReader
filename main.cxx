#include <iostream>
#include <fstream>
#include <cstring>

#include <itkCovariantVector.h>

//Mesh Information
typedef itk::CovariantVector<double,3> CovariantVectorType;


void ReadMesh(std::string _FileName);
void WriteLocationAttributeFile(std::string _FileName0, std::string _FileName1, std::string _FileName2);

int m_NbVertices;
std::vector<CovariantVectorType> m_vVertices;
  
int main (int argc, char * argv[])
{
  if (argc == 5)
    {
      ReadMesh(argv[1]);
      std::cout<<"Nb vertices: "<<m_NbVertices<<std::endl;
      WriteLocationAttributeFile(argv[2],argv[3],argv[4]);
      return EXIT_SUCCESS;
    }
  else
    {
      std::cerr<<"LocationMeshReader <InputMesh> <XLocationAttributeFile.txt> <YLocationAttributeFile.txt> <ZLocationAttributeFile.txt>"<<std::endl;
      return EXIT_FAILURE;
    }
}

void ReadMesh(std::string _FileName)
{
  std::cout<<"Reading mesh..."<<std::endl;
  
  std::ifstream Infile;
  std::string fileExtension, line;
  char Line[40], NbVertices[10];;
  std::size_t found1, found2, length;
  CovariantVectorType Vertex1, Vertex2, Vertex3;
  int CurrentPoint;
   
  m_vVertices.clear();  
  
  Infile.open(_FileName.c_str());  

  int lastPoint = _FileName.rfind('.');
  fileExtension = _FileName.substr(lastPoint);

  if (fileExtension.compare(".meta") == 0 )
    {
      //Skips the header and gets the number of points
      while ( strncmp (Line, "NPoints =", 9) && strncmp (Line, "NPoints=", 8))
	Infile.getline (Line, 40);    
      m_NbVertices = atoi(strrchr(Line,' '));
      
      Infile.getline ( Line, 40);

      //read the points in the file and set them as vertices
      for (int i = 0; i < m_NbVertices; i++ )
	{
	  Infile >> CurrentPoint >> Vertex1[0] >> Vertex1[1] >> Vertex1[2];
	  m_vVertices.push_back(Vertex1);
	}
    }
  else if (fileExtension.compare(".vtk") == 0 )
    {
      while ( strncmp (Line, "POINTS", 6))
	Infile.getline (Line, 40);
      line = Line;
      found1 = line.find(' ');
      found2 = line.find(' ',found1+1);
      length = line.copy(NbVertices,found2-found1-1,found1+1);
      NbVertices[length]='\0';
      m_NbVertices = atoi(NbVertices);
      
      for (int i = 0; i < m_NbVertices/3; i++ )
	{
	  Infile >> Vertex1[0] >> Vertex1[1] >> Vertex1[2] >> Vertex2[0] >> Vertex2[1] >> Vertex2[2] >> Vertex3[0] >> Vertex3[1] >> Vertex3[2];
	  m_vVertices.push_back(Vertex1);
	  m_vVertices.push_back(Vertex2);
	  m_vVertices.push_back(Vertex3);	  
	}
      if ((m_NbVertices % 3) == 1)
	{
	  Infile >> Vertex1[0] >> Vertex1[1] >> Vertex1[2];
	  m_vVertices.push_back(Vertex1);
	}
      else if ((m_NbVertices % 3) == 2)
	{
	  Infile >> Vertex1[0] >> Vertex1[1] >> Vertex1[2] >> Vertex2[0] >> Vertex2[1] >> Vertex2[2];
	  m_vVertices.push_back(Vertex1);
	  m_vVertices.push_back(Vertex2);
	}      
    }
  //close file
  Infile.close();
}

void WriteLocationAttributeFile(std::string _FileName0, std::string _FileName1, std::string _FileName2)
{
  int i;
  std::ofstream outfile0, outfile1, outfile2;
  
  outfile0.open(_FileName0.c_str());
  outfile1.open(_FileName1.c_str());
  outfile2.open(_FileName2.c_str());

  outfile0<<"NUMBER_OF_POINTS="<<m_NbVertices<<std::endl<<"DIMENSION=1"<<std::endl<<"TYPE=Scalar"<<std::endl;
  outfile1<<"NUMBER_OF_POINTS="<<m_NbVertices<<std::endl<<"DIMENSION=1"<<std::endl<<"TYPE=Scalar"<<std::endl;
  outfile2<<"NUMBER_OF_POINTS="<<m_NbVertices<<std::endl<<"DIMENSION=1"<<std::endl<<"TYPE=Scalar"<<std::endl;
  
  for ( i = 0; i < m_NbVertices; i++)
    {
      outfile0<<m_vVertices[i][0]<<std::endl;
      outfile1<<m_vVertices[i][1]<<std::endl;
      outfile2<<m_vVertices[i][2]<<std::endl;      
    }
  outfile0.close();
  outfile1.close();
  outfile2.close();  
}
