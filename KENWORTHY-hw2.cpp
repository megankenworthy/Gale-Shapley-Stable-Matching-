// Megan Kenworthy
// Gale-Shapley Algorithm
// 9 February 2018

#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <stack>
#include <vector>

using namespace std;

//function definition
void Gale_Shapley(int Pairs, map<string,string>&currentpairs, map<string,int>& Men, map<string,int>& Women, map<int,string>& Menreversed, map<int,string>& Womenreversed, vector<vector<string> >& Mpref,vector<vector<string> >& Wpref);


//Pre: Main's function is to read in the problem information
//Post: 4 maps: keys = male names, values = male ordering; keys = male ordering, values = male names
//		keys = female names, values = female ordering; keys = female ordering, values = female names
// 		2, 2-dimensional vectors: 1 holding male preferences, the other holding female preferences
//		A map of final pairings, used to print out the results 
int main()
{
	int numpairs;					//the number of pairings we will find
	cin >> numpairs;

	map<string,int> men;			//map, male name: index
	map<int,string> menreversed;	//map, male index: name
	map<string,int> women;			//map, female name: index
	map<int,string> womenreversed;	//map, index: female name

	map<string,string> finalpairs;	//map, name:name
	
	//in order of most to least preferencial 
	vector<vector<string> > malepreferences(numpairs); 		//2d-vector,[maleindex][femalename]
	vector<vector<string> > femalepreferences(numpairs);	//2d-vector,[femindex][malename]

	//Getting the male names and preferences 
	for(int i=0; i<numpairs; i++) 
	{
	 	string male_name;		//man's name
	 	cin >> male_name;
	 	
	 	men[male_name] = i+1;			//name:index
	 	menreversed[i+1] = male_name;	//index:name
	 
	 	malepreferences[i] = vector<string>(numpairs);	//initializing the second dimension

	 	//iterating through male preferences
	 	for(int p=0; p<numpairs; p++)
	 	{
	 		string preference;
	 		cin >> preference;					//female preference p 
	 		malepreferences[i][p] = preference;	//inserting into preference list 
	 	}
	}
	
	//getting all the female names and preferences 
	for(int f=0; f<numpairs; f++)
	{
	 	string female_name;					//female name 
	 	cin >> female_name;

	 	women[female_name] = f+1;			//name:index
	 	womenreversed[f+1] = female_name;	//index:name

	 	//to be used in gale-shapley, setting up each female as unpairs, value = "none"
	 	finalpairs[female_name] = "none";
	 	femalepreferences[f] = vector<string>(numpairs);	//initializing second dimension

	 	//iterating through female preferences
	 	for(int p=0; p<numpairs; p++)
	 	{	
	 		string preference;
	 		cin >> preference;						//female preference p
	 		femalepreferences[f][p] = preference;	//inserting into preference list
	 	}
	}

	//call Gale-Shapley
	Gale_Shapley(numpairs,finalpairs,men,women,menreversed, womenreversed, malepreferences,femalepreferences);

	//Print out pairs
	for(int toreturn=0; toreturn<numpairs; toreturn++)
	{
		string malename = menreversed[toreturn+1];	//in order men were inputted
		string femalepair;
		femalepair = finalpairs[malename];			//find female pair
		cout<<malename<<" "<<femalepair<<"\n";		//print "male female"
	}
	return 0;
}

//Pre: Pairs - the number of pairs to be made, currentpairs - a map of current pairings
//	   Men - map, malename:maleindex, Women - map, femalename:femaleindex
//	   Menreversed - map, maleindex:malename, Womenreversed - map, femaleindex:femalename
//	   Mpref - 2d vector, indexed male preferences ordered most to least preferencial
//	   Wpref - 2d vector, indexed female preferences ordered most to least preferencial
//Post: The currentpairs map with the correct key,value pairs 
void Gale_Shapley(int Pairs, map<string,string>&currentpairs, map<string,int>& Men, map<string,int>& Women, map<int,string>& Menreversed, map<int,string>& Womenreversed, vector<vector<string> >& Mpref, vector<vector<string> >& Wpref)
{

	//an array that hold the next index of the woman each man will propose to 
	int nextarray[Pairs]; 		
	
	//a stack holding the names of men who are still unpaired							
	stack<string> proposalstack; 
	
	//a reverse pair map that hold male name: female name 
	map<string,string> currentpairsreversed;
	
	//initializing every man's next proposal to be the first woman in their preference list
	for(int init=0;init<Pairs;init++)
	{
		nextarray[init] = 0;
	}

	//pushing men onto the stack in reverse order (so that the first man will be on top)
	for(int i=Pairs;i>0;i--)
	{
		string manonstack = Menreversed[i]; 
		proposalstack.push(manonstack);
	}

	//creating a inverse index for female preferences (so lookup will be in constant time)
	vector<vector<string> > femaleranking(Pairs); 
	for(int fpref=0;fpref<Pairs;fpref++) 		//for each woman 
	{
		femaleranking[fpref] = vector<string>(Pairs);
		for(int mname=0;mname<Pairs;mname++)    //iterating through her preferences
		{
			string malename = Wpref[fpref][mname];
			int maleindex = Men[malename];
			femaleranking[fpref][maleindex-1] = mname+1;  	//man's rank stored at his index 

		}
	}

	//while there is a man who is unpaired 
	while(proposalstack.empty() != true)
	{
		string manproposing = proposalstack.top(); 	//get the man proposing

		int manint = Men[manproposing];				
		int proposal = nextarray[manint-1]; 			//the int of the girl in man's pref list
		string womanname = Mpref[manint-1][proposal]; 	//woman to be asked

		int womanint = Women[womanname];
		string currentpartner = currentpairs[womanname];//woman's current partner

		int mancurrent = Men[currentpartner];			//int of current partner (to check for pref)

		if(currentpartner == "none")					//if the woman is unpaired she
		{												//will always accept
			proposalstack.pop();						//man will be taken off proposal stack
			currentpairs[womanname] = manproposing;		//this is the new pairs

			nextarray[manint-1] +=1;					//increment his next proposal

			currentpairsreversed[manproposing] = womanname;
		}

		//if the woman is paired but prefers the man proposing to her current partner
		else if((femaleranking[womanint-1][manint-1])<(femaleranking[womanint-1][mancurrent-1]))
		{
			proposalstack.pop();						//pop the man proposing off the stack
			currentpairs[womanname] = manproposing;		//assign the new pair
			proposalstack.push(currentpartner);			//push the man she broke up w to top
			nextarray[manint-1]+=1;						//increment her new partner's next proposal

			currentpairsreversed[manproposing] = womanname;
		}
		else
		{												//if the man is rejected, he stays 
			nextarray[manint-1]+=1; 					//at top of proposal list and proposes
		}												//to next women 
	}
	currentpairs = currentpairsreversed;
}