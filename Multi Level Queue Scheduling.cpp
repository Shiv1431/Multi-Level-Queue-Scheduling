#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

// Use VS code to format code 
// Structure to represent a process
struct Process
{
    int ProcessID;         // Process ID
    int BurstTime;   // Burst time of the process
    int TimePeriod;  // Time period of the process (for Queue 1)
    int arrivalTime; // Arrival time of the process (for Queue 2)
};

// Function to execute a process with a given burst time
void executeProcess(Process p, int currentTime)
{
    cout << "Executing Process P" << p.ProcessID << " at Time " << currentTime << endl;
}

int calculateLCM(int a, int b)
    {
        int gcd = 1;
        for (int i = 1; i <= a && i <= b; ++i)
        {
            if (a % i == 0 && b % i == 0)
            {
                gcd = i;
            }
        }
        return (a * b) / gcd;
    }

int main()
{
    queue<Process> q1;   // Queue 1 (Rate Monotonic)
    queue<Process> q2;   // Queue 2 (Round Robin)
    int currentTime = 0; // Current time  (Used in execution time finding)
    int QuantumTime;     // Quantum time for Queue 2(round robin)

    cout << "Enter Quantum Time for Queue 2 (Round Robin): ";
    cin >> QuantumTime;

    // Loop to take input for processes
    while (true)
    {
        int type;
        cout << "Enter Type of Process (1 for Queue 1, 2 for Queue 2, -1 to exit): ";
        cin >> type;

        if (type == -1)
        {
            break; // Exit loop if -1 is entered
        }

        Process p;
        p.ProcessID = (type == 1) ? q1.size() + 1 : q2.size() + 1;

        if (type == 1)
        {
            cout << "Enter Burst Time for Process P" << p.ProcessID << ": ";
            cin >> p.BurstTime;
            cout << "Enter Time Period for Process P" << p.ProcessID << ": ";
            cin >> p.TimePeriod;
            q1.push(p);
        }
        else
        {
            cout << "Enter Arrival Time for Process P" << p.ProcessID << ": ";
            cin >> p.arrivalTime;
            cout << "Enter Burst Time for Process P" << p.ProcessID << ": ";
            cin >> p.BurstTime;
            q2.push(p);
        }
    }

    // Sort Queue 1 based on time period in ascending order
    int q1Size = q1.size();
    Process q1Array[q1Size];
    for (int i = 0; i < q1Size; i++)
    {
        q1Array[i] = q1.front();
        q1.pop();
    }
    sort(q1Array, q1Array + q1Size, [](const Process &p1, const Process &p2)
         { return p1.TimePeriod < p2.TimePeriod; });

    

    for (int i = 0; i < q1Size; i++)
    {
        q1.push(q1Array[i]);
    }

    int q2Size = q2.size();
    int timeQuantum = 2;
    int q2Timer = 0;
    int q2Index = 0;
    int lcm = calculateLCM(timeQuantum, q1.front().TimePeriod);

    cout << "Gantt Chart: ";
    // Execute processes in Round Robin (Queue 2) until both queues are empty
    while (!q1.empty() || !q2.empty())
    {
        // Check for processes in Queue 1 (Rate Monotonic)
        if (!q1.empty())
        {
            Process p1 = q1.front();
            if (p1.TimePeriod <= q2Timer)
            {
                q1.pop();
                executeProcess(p1, q2Timer);
                q2Timer = 0;
                q2.push(p1);
            }
        }

        // Check for processes in Queue 2 (Round Robin)
        if (!q2.empty())
        {
            Process p2 = q2.front();
            q2.pop();
            int remainingBurstTime = p2.BurstTime;
            while (remainingBurstTime > 0)
            {
                // Execute process with time quantum or remaining burst time, whichever is smaller
                int executionTime = min(remainingBurstTime, timeQuantum);
                q2Timer += executionTime;
                remainingBurstTime -= executionTime;
                executeProcess(p2, q2Timer);

                // Check if any processes from Queue 1 (Rate Monotonic) have arrived during execution
                if (!q1.empty() && q1.front().TimePeriod <= q2Timer)
                {
                    Process p1 = q1.front();
                    q1.pop();
                    executeProcess(p1, q2Timer);
                    q2Timer = 0;
                    q2.push(p1);
                }

                // Check if the current process from Queue 2 (Round Robin) has remaining burst time
                if (remainingBurstTime > 0)
                {
                    q2.push(p2);
                }
            }
        }
    }

    cout << endl;

    return 0;
}
