#include <iostream>
#include <mutex>

struct Node
{
    int value;
    Node* next;
    std::mutex* node_mutex;

    Node(int val) : value(val), next(NULL), node_mutex(new std::mutex) {}
    ~Node()
    {
        delete node_mutex;
    }
};

class FineGrainedQueue
{
    public:
    FineGrainedQueue()
    {
        head = new Node(0);
    }

    ~FineGrainedQueue()
    {
        Node* current = head;
        while (current)
        {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
    }

    void insertIntoMiddle(int value, int pos)
    {
        Node* newNode = new Node{value};

        Node* prev = NULL;
        Node* current = head;

        current->node_mutex->lock();

        int currPos = 0;

        while (current->next && currPos < pos - 1)
        {
            Node* temp = current->next;
            temp->node_mutex->lock();

            current->node_mutex->unlock();

            current = temp;
            currPos++;
        }

        newNode->next = current->next;
        current->next = newNode;

        current->node_mutex->unlock();
    }

    void show()
    {
        Node* current = head->next;
        while (current)
        {
            std::cout << current->value << " "; 
            current = current->next;
        }
        std::cout << std::endl;
    }

    private:
    Node* head;
    
};

int main()
{
    FineGrainedQueue m;

    m.insertIntoMiddle(1, 0);
    m.insertIntoMiddle(2, 1);
    m.insertIntoMiddle(3, 2);
    m.insertIntoMiddle(7, 3);
    m.show();

    return 0;
}
