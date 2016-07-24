#include<iostream>
#include<ctime>
#include<unistd.h>
#include<cstdlib>
#include<cstdio>
#include<cmath>

using namespace std;

int press_enter;
int random_index(int);

class Game;

class Game_AI
{

  int response;
  int apocalypse;
  char control;
  
public:
  
  int max;
  int win;
  int plus;
  int score;
  int grid[4][4];
  int bgrid[4][4];
  
  Game_AI(): response(0),apocalypse(1),max(0),win(2048),plus(0),score(0) {}
  
  void logic_flow(Game*);
  void game_end_check(Game*);
  void key_press();
  void start_grid();
  void update_grid();
  void fill_space();
  void spawn();
  void find_greatest_tile();
  void backup_grid();
  void undo();
  
  int full();
  int block_moves();
};

class Game : public Game_AI
{
  char option;
  string name;
  
public:
  void display_grid();
  void display_help_screen();
  void display_win_screen();
  void display_loser_screen();
  char display_try_again_screen(int);

};

int main()
{
  Game exec;
  
  srand(time(NULL));

  exec.start_grid();
  
    while(1)
    {
      exec.display_grid();
      exec.key_press();
      exec.logic_flow(&exec);
      exec.game_end_check(&exec);
    };
    
return 0;
}

void Game_AI::key_press()
{
  system("stty raw");
  cin>>control;
  system("stty cooked");
}


void Game_AI::logic_flow(Game *execute)
{
  
  switch(control)
  {
      case 'w':
      case 'a':
      case 's':
      case 'd':
		  execute->backup_grid();
		  execute->fill_space();
		  execute->update_grid();
		  execute->fill_space();
		  execute->find_greatest_tile();
		  execute->display_grid();
		  usleep(1000*160);

		  if(execute->full()&&apocalypse)
		  {
		      response=-1;
		      break;
		  }
		  else if(execute->block_moves())
		  {
		      execute->spawn();
		      break;
		  }
		  else
		  {
		      response=0;	
		      break;
		  }

      case 'u':	
		  if(execute->block_moves())
		    score-=plus;
		  execute->undo();
		  break;
						  
						  
      case 'r': 	
		  execute->start_grid();
		  score=0;
		  plus=0;
		  break;

      case 'q':		
		  response=-1;
		  break;
			  
      case 'h': 	
		  execute->display_help_screen();
		  press_enter=getchar();
		  execute->display_grid();
		  break;
		  
  }
}

void Game_AI::game_end_check(Game *screen)
{
  if(max==win)
  {
    screen->display_win_screen();
    
    if(screen->display_try_again_screen(0)=='n')
	response=-1;
    else
	win*=2;
  }
  
  else if(response==-1)
  {
    screen->display_loser_screen();
  
    if(screen->display_try_again_screen(1)=='y')
    {
	screen->start_grid();
	response=0;
    }
  }
    
  if(response==-1)
  {
    cout<<"\n\n\t\t            > Thank you for playing. "
        <<"\n\n\n\t\t\t\t\t\t   DEVELOPED BY SCM [-_-] \n\n";
    press_enter=getchar();
    exit(0);
  }
}

void Game_AI::start_grid()
{
  int i,j;
  
  plus=0;
  score=0;
  max=0;
  
	for(i=0;i<4;i++)	
		for(j=0;j<4;j++)
			grid[i][j]=0;

	i=random_index(4);
	j=random_index(4);

		grid[i][j]=2;

	i=random_index(4);
	j=random_index(4);

		grid[i][j]=2;
}

void Game::display_grid()
{
	system("clear");
			
		cout<<"\n  ::[  THE 2048 PUZZLE  ]::\t\t\t\tDeveloped by SCM [-_-]\n\n\t";
		
			if(plus)
				cout<<"+"<<plus<<"!";
			else
				cout<<"   ";
			
		cout<<"\t\t\t\t\t\tSCORE::"<<score<<" \n\n\n\n";
				
				for(int i=0;i<4;i++)
				{
				cout<<"\t\t     |";

					for(int j=0;j<4;j++)
					{
						if(grid[i][j])
							printf("%4d    |",grid[i][j]);
						else
							printf("%4c    |",' ');
					} 

				cout<<endl<<endl;
				}

		cout<<"\n\n Controls (+ :: o)\t\t\t\tu - undo\tr - restart\n\n\tW\t\t     ^\t\t\th - help\tq - quit\n\t\t\t\t\t\t\t\t"
		    <<" \n   A    S    D\t\t<    v    >\t\t\t     ."
		    <<" \n\t\t\t\t                             ";
		
}

int random_index(int x)
{
    int index;
    index=rand()%x+0;

return index;
}

void Game_AI::backup_grid()
{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				bgrid[i][j]=grid[i][j];
}

		
void Game_AI::fill_space()
{

	switch(control)
	{
		case 'w':
					for(int i=0;i<4;i++)
						for(int j=0;j<4;j++)
						{
							if(!grid[j][i])
							{
								for(int k=j+1;k<4;k++)
									if(grid[k][i])
									{
										grid[j][i]=grid[k][i];
										grid[k][i]=0;
										break;
									}
							}

						}break;

		case 's':
					for(int i=0;i<4;i++)
						for(int j=3;j>=0;j--)
						{
							if(!grid[j][i])
							{
								for(int k=j-1;k>=0;k--)
									if(grid[k][i])
									{
										grid[j][i]=grid[k][i];
										grid[k][i]=0;
										break;
									}
							}

						}break;
		case 'a':
					for(int i=0;i<4;i++)
						for(int j=0;j<4;j++)
						{
							if(!grid[i][j])
							{
								for(int k=j+1;k<4;k++)
									if(grid[i][k])
									{
										grid[i][j]=grid[i][k];
										grid[i][k]=0;
										break;
									}
							}

						}break;


		case 'd':
					for(int i=0;i<4;i++)
						for(int j=3;j>=0;j--)
						{
							if(!grid[i][j])
							{
								for(int k=j-1;k>=0;k--)
									if(grid[i][k])
									{
										grid[i][j]=grid[i][k];
										grid[i][k]=0;
										break;
									}
							}

						}break; 

	}
}


void Game_AI::update_grid()
{
	plus=0;
	apocalypse=1;

	switch(control)
	{
		case 'w':
					for(int i=0;i<4;i++)
						for(int j=0;j<3;j++)
						{
							if(grid[j][i]&&grid[j][i]==grid[j+1][i])
							{
								apocalypse=0;
								grid[j][i]+=grid[j+1][i];
								grid[j+1][i]=0;
								plus+=(((log2(grid[j][i]))-1)*grid[j][i]);
								score+=(((log2(grid[j][i]))-1)*grid[j][i]);
								
							}
						}break;

		case 's':
					for(int i=0;i<4;i++)
						for(int j=3;j>0;j--)
						{
							if(grid[j][i]&&grid[j][i]==grid[j-1][i])
							{
								apocalypse=0;
								grid[j][i]+=grid[j-1][i];
								grid[j-1][i]=0;
								plus+=(((log2(grid[j][i]))-1)*grid[j][i]);
								score+=(((log2(grid[j][i]))-1)*grid[j][i]);
							}
						}break;

		case 'a':
					for(int i=0;i<4;i++)
						for(int j=0;j<3;j++)
						{
							if(grid[i][j]&&grid[i][j]==grid[i][j+1])
							{
								apocalypse=0;
								grid[i][j]+=grid[i][j+1];
								grid[i][j+1]=0;
								plus+=((log2(grid[i][j]))-1)*grid[i][j];
								score+=((log2(grid[i][j]))-1)*grid[i][j];
							}
						}break;

		case 'd':
					for(int i=0;i<4;i++)
						for(int j=3;j>0;j--)
						{
							if(grid[i][j]&&grid[i][j]==grid[i][j-1])
							{
								apocalypse=0;
								grid[i][j]+=grid[i][j-1];
								grid[i][j-1]=0;
								plus+=((log2(grid[i][j]))-1)*grid[i][j];
								score+=(((log2(grid[i][j]))-1)*grid[i][j]);
							}
						}break;


	}
	
	

}


void Game_AI::spawn()
{
		int i,j,k;
		
		do
		{
		    i=random_index(4);
		    j=random_index(4);
		    k=random_index(10);

		}while(grid[i][j]);
				
		
			
		if(k<2)
		    grid[i][j]=4;

		else
		    grid[i][j]=2;
	

}
		
		
void Game_AI::find_greatest_tile()
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(grid[i][j]>max)
				max=grid[i][j];
}

int Game_AI::full()
{
	int k=1;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if(!grid[i][j])
				k=0;
				
		}
return k;
}

void Game::display_help_screen()
{
	system("clear");
	
	cout<<endl<<"  ::[  THE 2048 PUZZLE  ]::\t\t\t\tDeveloped by SCM [-_-]";
	
	char text[]=">The game starts with 1 or 2 randomly placed numbers in a 4x4 grid (16 cells).> Use the controls to move the cells. ( Press W/A/S/D )> 2 cells of the same number will merge and add up.> New 2s and 4s will appear randomly on an empty cell with each move you make.> Your objective is to make 2048 in a cell without getting stuck!> Press Enter to continue...";

	for(int i=0; text[i] ; i++)
	{
	    if(text[i]=='>')
	    {
	    usleep(1000*500);
	    cout<<"\n\n\n";
	    }
    
	    cout<<text[i];
	}
}
	

void Game::display_win_screen()
{
	system("clear");
	
	cout<<endl<<endl;
	cout<<"\n\t\t\t  :: [  YOU MADE "<<win<<"!  ] ::"
	    <<"\n\n\t\t\t  :: [ YOU WON THE GAME ] ::"
	    <<"\n\n\n\n\t\t\t TILE\t     SCORE\t    NAME";
	printf("\n\n\t\t\t %4d\t    %6d\t    ",max,score);
	cin>>name;

	cout<<"\n\n\t\t> The maximum possible tile is 65,536 ! So go on :)";
}

void Game::display_loser_screen()
{
	system("clear");
	
	cout<<"\n\n\n\t\t\t  :: [ G A M E  O V E R ] ::"
	    <<"\n\n\n\n\t\t\t TILE\t     SCORE\t    NAME";
	printf("\n\n\t\t\t %4d\t    %6d\t    ",max,score);
	cin>>name;

	cout<<"\n\n\t\t> The maximum possible score is 3,932,156 ! So close :p";
}

char Game::display_try_again_screen(int lose)
{	
	if(lose)
	cout<<"\n\n\n\t    > Would you like to try again "<<name<<" (y/n) ? :: ";
	else
	cout<<"\n\n\n\t    > Would you like to continue playing "<<name<<" (y/n) ? :: ";
	
	system("stty raw");
	cin>>option;
	system("stty cooked");
	
return option;
}
  
void Game_AI::undo()
{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				grid[i][j]=bgrid[i][j];
}

int Game_AI::block_moves()
{
	int k=0;

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(bgrid[i][j]!=grid[i][j])
			{	
				k=1;
				break;
			}
return k;
}				
					


