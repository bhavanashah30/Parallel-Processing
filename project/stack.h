class Stack {
   int MaxStack;
   int EmptyStack;
   int top;
   char* items;
public:
   Stack(int);
   ~Stack();
   void push(char);
   char pop();
   int empty();
   int full();
};