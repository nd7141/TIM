//#define HEAD_TRACE
#define HEAD_INFO

#define HEAD_INFO
//#define HEAD_TRACE
#include "sfmt/SFMT.h"
#include "head.h"
#include "memoryusage.h"
#include "graph.h"

#include <iostream>
#include <iomanip>

void run(TimGraph & m, string dataset, int k, double epsilon, string model,
 string output_seeds, string output_runtime, string output_memory){
    cout << "dataset:" << dataset << " k:" << k << " epsilon:"<< epsilon <<   " model:" << model << endl;
    m.k=k;
    if(model=="IC")
        m.setInfuModel(InfGraph::IC);
    else if(model=="LT")
        m.setInfuModel(InfGraph::LT);
    else
        ASSERT(false);

    cout<<"Finish Read Graph, Start Influence Maximization"<<endl;
    m.EstimateOPT(epsilon, output_memory);
    cout<<"Time used: " << Timer::timeUsed[100]/TIMES_PER_SEC << "s" <<endl;
    Timer::clearAll();

    // cout<<"Selected k SeedSet: ";
    // for(auto item:m.seedSet)
    //     cout<< item << " ";
    // cout<<endl;

    std::ofstream myfile;
    // write running time
    myfile.open(output_runtime.c_str(), ios_base::app);
    myfile << epsilon << " " << Timer::timeUsed[100]/TIMES_PER_SEC << endl;
    myfile.close();
    // write seeds
    myfile.open(output_seeds.c_str(), ios_base::app);
    myfile << epsilon << " ";
    for(auto item:m.seedSet)
        myfile << item << " ";
    myfile << endl;
    myfile.close();
    cout << endl;
    cout << endl;
    cout<<"Finish writing to file...";
    cout<<endl;

    // TODO create folder for runtime/memory and check if it writes to those files

    // cout<<"Estimated Influence: " << m.InfluenceHyperGraph() << endl;
    Counter::show();
}

template <typename T>
std::string to_str(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

void parseArg(int argn, char ** argv)
{
    string dataset="";

    double epsilon=0;
    string model="";
    int k=0;

    for(int i=0; i<argn; i++)
    {
        if(argv[i]==string("-dataset")) dataset=string(argv[i+1])+"/";
        if(argv[i]==string("-epsilon")) epsilon=atof(argv[i+1]);
        if(argv[i]==string("-k")) k=atoi(argv[i+1]);
        if(argv[i]==string("-model")) {
            if(argv[i+1]==string("LT"))
            {
                model=argv[i+1];
            }
            else if(argv[i+1]==string("IC"))
            {
                model=argv[i+1];
            }
            else
                ExitMessage("model should be IC or LT");
        }
    }
    if (dataset=="")
        ExitMessage("argument dataset missing");
    if (k==0)
        ExitMessage("argument k missing");
    if (epsilon==0)
        ExitMessage("argument epsilon missing");
    if (model=="")
        ExitMessage("argument model missing");

    string graph_file;
    string graph_folder; 
    // for (int K = 100; K<=100; K+=10) {
    //     graph_folder = dataset + "K" + to_string(K) + "/";
    //     if(model=="IC") {
    //         graph_file= graph_folder + "graph_ic.inf";
    //     }
    //     else if(model=="LT")
    //         graph_file=graph_folder + "graph_lt.inf";

    //     cout << graph_file << endl;

    //     TimGraph m(graph_folder, graph_file);
    //     for (int k=50;k<=50;k+=10) {
    //         cout << k << endl;
    //         string output = dataset + "Seeds/K" + to_string(K) +"_k" + to_string(k) + ".txt";
    //         run(m, graph_folder, k ,  epsilon, model, output);
    //     }
    //     cout << "******************" << endl;
    //     cout << endl;
    // }

    vector<int> range;
    range.push_back(20);
    range.push_back(30);
    range.push_back(40);
    range.push_back(50);
    range.push_back(100);
    for (double eps=0.09; eps >= 0.09; eps -= 0.05) {
        for (int K: range) {
            string output_seeds = dataset + "Seeds/K" + to_string(K) + ".txt"; //+"_eps" + to_str(eps, 2) + ".txt";
            string output_memory = dataset + "Memory/K" + to_string(K) + ".txt"; //+"_eps" + to_str(eps, 2) + ".txt";
            string output_runtime = dataset + "Runtime/K" + to_string(K) + ".txt"; //+"_eps" + to_str(eps, 2) + ".txt";            
            graph_folder = dataset + "K" + to_string(K) + "/";
            if(model=="IC") {
                graph_file= graph_folder + "graph_ic.inf";
            }
            else if(model=="LT")
                graph_file=graph_folder + "graph_lt.inf";

            cout << graph_file << endl;

            TimGraph m(graph_folder, graph_file);
            run(m, graph_folder, k,  eps, model, output_seeds, output_runtime, output_memory);
        }
        cout << "******************" << endl;
        cout << endl;
    }
    
}

int main(int argn, char ** argv)
{
    OutputInfo info(argn, argv);
    parseArg( argn, argv );
}






