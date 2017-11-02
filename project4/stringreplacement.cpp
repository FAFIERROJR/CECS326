#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h> 

using namespace std;

//declare methods
string getReplacementString();
string getTargetString();

int main(){

	//the child's process id
	int pid = 0;

	string text = "Greetings, everyone. I am new. (One second – let me get this spork out of the way.) My name is Katy, but you can call me the Penguin of Doom. (I’m laughing aloud.) As you can plainly see, my actions have no pattern whatsoever. That is why I have come here. To meet similarly patternless individuals, such as myself. I am 13 – mature for my age, however! – and I enjoy watching Invader Zim with my girlfriend. (I am bisexual. Please approach this subject maturely.) It is our favorite television show, as it adequately displays stochastic manners of behavior such as we possess. She behaves without order – of course – but I wish to meet more individuals of her and my kind. As the saying goes, \"the more, the merrier.\" Ah, it is to laugh. Anyway, I hope to make many friends here, so please comment freely. Doom! That is simply one of many examples of my random actions. Ha, ha. Fare thee well. I wish you much love and waffles. Yours, The Penguin of Doom. To be fair, you have to have a very high IQ to understand Rick and Morty. The humor is extremely subtle, and without a solid grasp of theoretical physics most of the jokes will go over a typical viewer's head. There's also Rick's nihilistic outlook, which is deftly woven into his characterisation - his personal philosophy draws heavily from Narodnaya Volya literature, for instance. The fans understand this stuff; they have the intellectual capacity to truly appreciate the depths of these jokes, to realize that they're not just funny- they say something deep about LIFE. As a consequence people who dislike Rick and Morty truly ARE idiots- of course they wouldn't appreciate, for instance, the humour in Rick's existential catchphrase \"Wubba Lubba Dub Dub\", which itself is a cryptic reference to Turgenev's Russian epic Fathers and Sons I'm smirking right now just imagining one of those addlepated simpletons scratching their heads in confusion as Dan Harmon's genius unfolds itself on their television screens. What fools... how I pity them. And yes by the way, I have a Rick and Morty tattoo. And no, you cannot see it. It's for the ladies' eyes only- And even they have to demonstrate that they're within 5 IQ points of my own (preferably lower) beforehand.";

	//s.erase (std::remove_if(s.begin (), s.end (), ispunct), s.end ());

	string exitString = "lwq";


	int replacementInstancesCount;
	string replacement;
	string target;
	do{
		replacementInstancesCount = 0;

		//ask for replacement and target string
		replacement = getReplacementString();
		if(replacement.compare(exitString) == 0){
			break;
		}
		
		target = getTargetString();

		//if either entry is the exit code, break
		if(target.compare(exitString) == 0){
			break;
		}

		//create a child process
		pid = fork();

		//skip if you're the parent
		if(pid == 0){
			int beginIndex = 0;
			int endIndex = 0;
			replacementInstancesCount = 0;
			string textLeftEnd;
			string textRightEnd;
			while(text.find(target) != -1){
				beginIndex = text.find(target);
				endIndex = beginIndex + target.length()- 1;
				textLeftEnd = text.substr(0, beginIndex);
				textRightEnd = text.substr(endIndex + 1);
				text = textLeftEnd + replacement + textRightEnd;
				replacementInstancesCount++;
			}

			cout << text << endl;
			cout << replacementInstancesCount << " instances of the word " << target << " were replaced by " << replacement << endl;
		}

		else if(pid > 0){
			int status;
			wait(&status);
  		}

	}while(pid > 0);

	exit(0);
}

string getReplacementString(){
	string replacement;
	cout << "Enter the replacement string" << endl;
	getline(cin, replacement);
	return replacement;
}

string getTargetString(){
	string target;
	cout << "Enter the target string" << endl;
	getline(cin, target);
	return target;
}
