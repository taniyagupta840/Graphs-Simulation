#include <windows.h> 
#include <GL/freeglut.h>
#include <fstream> 
#include <iostream>
#include <math.h>
#include <sstream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define V 46

using namespace std;

//void timer(int);
int getTheClient();
void readData();
void *takeInput(void *);

int obstruct;

struct Edge{
	int src, dest;
}edges[130];

float AdjMatrix[V][V];

struct Vertices{
	bool isThere = true;
	int num;
	float x, y;
}v[V];

class Graph{
	public :
		Graph(Edge edges[], int n){
			int i,j;
			for(i=0; i<V; i++){
				for(j=0; j<V; j++){
					AdjMatrix[i][j] = 0;
				}
			}
			
			for(i=0; i<n; i++){
				int src = edges[i].src;
				int dest = edges[i].dest;
				AdjMatrix[src][dest] = sqrt(pow(v[src].x-v[dest].x,2) + pow(v[src].y-v[dest].y,2));
				AdjMatrix[dest][src] = AdjMatrix[src][dest];
			}
		}
};

void printPath(int, int);
int parent[V];
int src=0, dest=0;

int MinDist(float dist[], bool sptSet[]){
	int min = INT_MAX, minIndex;
	for(int i=0; i<V; i++){
		if (!sptSet[i] && dist[i] < min && v[i].isThere){
			min = dist[i];
			minIndex = i;
		}
	}
	return minIndex;
}

float dist[V];

void dijsktra(float graph[V][V], int src, int dest){
	bool sptSet[V];
	int i;
	for(i=0; i<V; i++){
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}
	
	dist[src] = 0;
	parent[src] = src;
	for(i=0; i<V-1; i++){
		int u = MinDist(dist, sptSet);
		sptSet[u] = true;
		//if(sptSet[dest] == true)
		//   break;
		for(int j=0; j<V; j++){
			if(dist[u] + graph[u][j] < dist[j] && graph[u][j] && dist[u] != INT_MAX){
			    dist[j] = dist[u] + graph[u][j];
			    parent[j] = u;
			}
		}
	}
	cout<<"Distance : "<<dist[dest]<<endl;
	cout<<"Path : "<<src;
	printPath(dest, src);
	cout<<endl;
	//timer(0);
}

void printPath(int j, int src){
	if(j==src){
		return;
	}
	printPath(parent[j], src);
	cout<<"->"<<j;
}

char title[] = "Map";
 
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClearDepth(1.0f);                   
    glEnable(GL_DEPTH_TEST);   
    glDepthFunc(GL_LEQUAL);    
    glShadeModel(GL_SMOOTH);   
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
}
 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);     
 
    glLoadIdentity();       
    glPointSize(8);          
    glTranslatef(1.5f, 0.0f, -7.0f);  
   
    for(int i=0; i<V; i++){
   	    glColor3d(0.0, 1.0, 0.0);
		stringstream ss;
        ss << i;
        const unsigned char *p = (const unsigned char*)ss.str().c_str();
		glRasterPos2f((float)(::v[i].x-0.20), (float)::v[i].y+0.1);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, p);
    }
    glColor3d(1.0, 1.0, 1.0);
    char ss[18] = "Distance = ";
    stringstream dist1;
	dist1 << dist[dest];
	strcat(ss, dist1.str().c_str());
    const unsigned char *p = (const unsigned char*)ss;
	glRasterPos2f(-6.75f, -2.75f);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, p);
   
    glBegin(GL_LINES);                
        glColor3f(1.0f, 1.0f, 1.0f);   
	    for(int i=0; i<130; i++){
	    	glVertex3f((float)v[edges[i].src].x, (float)v[edges[i].src].y, 0.0f);
	    	glVertex3f((float)v[edges[i].dest].x, (float)v[edges[i].dest].y, 0.0f);
		}
    glEnd();
   
    glBegin(GL_POINTS);
	  
	    glColor3f( 0.0f, 0.0f, 1.0f);
	  
	    for(int i=0; i<V; i++){
	  	    glVertex3f((float)::v[i].x, (float)::v[i].y, 0.0f);
	    }
	  
    glEnd();
   
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);    
		int j = dest;
        while(j!=src){
        	glVertex3f((float)v[j].x, (float)v[j].y, 0.0f);
	    	glVertex3f((float)v[parent[j]].x, (float)v[parent[j]].y, 0.0f);
	    	j = parent[j];
		}
    glEnd();
   
    glBegin(GL_POINTS);
    glLineWidth(3.0f);
        glColor3f(1.0f, 1.0f, 1.0f);    
		j = dest;
        do{
            glVertex3f((float)v[j].x, (float)v[j].y, 0.0f);
	    	glVertex3f((float)v[parent[j]].x, (float)v[parent[j]].y, 0.0f);
	    	j = parent[j];
		}while(j!=src);
    glEnd();   

    glLoadIdentity();                  
    glTranslatef(-1.5f, 0.0f, -6.0f);  
 
    glutSwapBuffers();
}
 
void reshape(GLsizei width, GLsizei height) {  
    if (height == 0) height = 1;                
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();             
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
    
    readData();
    
    pthread_t thread;
    pthread_create( &thread, NULL, takeInput, NULL);
	
	glutInit(&argc, argv);           
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(1400, 750);  
    glutInitWindowPosition(0, 0); 
    glutCreateWindow(title);       
    glutDisplayFunc(display);      
    glutReshapeFunc(reshape);
    //glutTimerFunc(500, timer, 0);      
    initGL();
	                    
    glutMainLoop();   
	         
    return 0;
}

void timer(int){
	//takeInput();  
	glutPostRedisplay();
	glutTimerFunc(1000/40, timer, 0);
}

void readData(){
    ifstream mapFile("mapData.mp");
    ifstream neigh("mapNeigh.mp");
    int i,j;
    for(i=0; i<V; i++){
        mapFile >> v[i].num>>v[i].x>>v[i].y;
    }   
    for(i=0; i<130; i++){
   	    neigh>>edges[i].src>>edges[i].dest;
    }
    /*
    for(i=0; i<130; i++){
   	    cout<<edges[i].src<<" "<<edges[i].dest<<endl;
    }
    */
    Graph graph(edges, 130);
    /*
    for(i=0; i<V; i++){
        for(j=0; j<V; j++){\
	        cout<<AdjMatrix[i][j]<<" ";
	    }
	    cout<<endl;
    }
    */	
}

void *takeInput(void *){
	cout<<"Source : ";
    cin>>src;
    cout<<"Destination : ";
    cin>>dest;
	dijsktra(AdjMatrix, src, dest);
	/*
	for(int j=0; j<V; j++){
    	cout<<endl<<parent[j];
	}
	*/
	getTheClient();
}

int getTheClient(){
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char server_reply[4000];
	int recv_size;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(s , (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	else{
	puts("Connected");
	}
	//Send some data	
	char message[] = "Update Regarding Obstacles.";
	//message[strlen(message)] = ''
	if( send(s , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	else{
	    puts("Data Send\n");
	}
	//Receive a reply from the server
	if((recv_size = recv(s , server_reply , 2 , 0)) != SOCKET_ERROR){
			obstruct = atoi(server_reply);
	        printf("Obstruction at point %d\n", obstruct);
	        //Add a NULL terminating character to make it a proper string before printing
	        //server_reply[recv_size] = '\0';
	        v[obstruct].isThere = false;
	    
	    cout<<"Obstructions Updated...\n";
    }
	dijsktra(AdjMatrix, src, dest);
}
