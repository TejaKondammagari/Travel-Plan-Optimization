//
//  main.cpp
//  Project1
//
//  Created by Teja  on 2/3/19.
//  Copyright © 2019 Teja . All rights reserved.
//

//#ifndef _TRVL_OPTNS_H
#define _TRVL_OPTNS_H

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class TravelOptions{
    
public:
    enum Relationship { better, worse, equal, incomparable};
    
private:
    struct Node {
        double price;
        double time;
        Node *next;
        
        Node(double _price=0, double _time=0, Node* _next=nullptr){
            price = _price; time = _time; next = _next;
        }
        
    };
    
    /* TravelOptions private data members */
    Node *front;  // pointer for first node in linked list (or null if list is empty)
    int _size;
    
public:
    // constructors
    TravelOptions() {
        front = nullptr;
        _size=0;
    }
    
    ~TravelOptions( ) {
        clear();
    }
    
    
    /**
     * func: clear
     * desc: Deletes all Nodes currently in the list
     * status:  DONE
     */
    
    void clear(){
        Node *p, *pnxt;
        p = front;
        while(p != nullptr) {
            pnxt = p->next;
            delete p;
            p = pnxt;
        }
        _size = 0;
        front = nullptr;
    }
    
    
    /**
     * func: size
     * desc: returns the number of elements in the list
     * status:  DONE
     */
    int size( ) const {
        return _size;
    }
    //----------------------------------------------------------------------------------------------------------
    
    /**
     * function: compare
     * description: compares option A (priceA, timeA) with option B (priceB, timeA) and
     *        returns result (see enum Relationship above):
     *
     *       There are four possible scenarios:
     *        - A and B are identical:  option A and option B have identical price and time:
     *                     ACTION:  return equal
     *        - A is better than B:  option A and B are NOT equal/identical AND
     *                    option A is no more expensive than option B AND
     *                    option A is no slower than option B
     *                    ACTION:  return better
     *        - A is worse than B:  option A and B are NOT equal/identical AND
     *                    option A is at least as expensive as option B AND
     *                    option A is no faster than option B
     *                    ACTION:  return worse
     *                    NOTE:  this means B is better than A
     *        - A and B are incomparable:  everything else:  one option is cheaper and
     *                          the other is faster.
     *                    ACTION:  return incomparable
     *
     * COMMENTS:  since this is a static function, there is no calling object.
     *            To call it from a client program, you would do something like this:
     *
     TravelOptions::Relationship r;
     
     double pa, ta, pb, tb;
     
     // some code to set the four price/time variables
     
     r = TravelOptions::compare(pa, ta, pb, tb);
     if(r == TravelOptions::better)
     std::cout << "looks like option b is useless!" << std::endl;
     // etcetera
     *
     * status: DONE
     */
    
    static Relationship compare(double priceA, double timeA,
                                double priceB, double timeB) {
        
        // If A and B have the same time and price, returns that both travel options are equal
        if (priceA == priceB && timeA == timeB){
            return equal;
        }
        // If A is better than B in both time and price aspects
        if (priceA + timeA < priceB + timeB){
            return better;
        }
        // If B is better than A in both time and price aspects
        if (priceA + timeA > priceB + timeB){
            return worse;
        }
        // If one option is cheaper but the other option is faster
        return incomparable;  // placeholder
    }
    // MY STATUS: I think its done, need to be cross checked
    
    //---------------------------------------------------------------------------------------------------------
    
private:
    
    /**
     * function: compare(Node*, Node*)
     * description: private utilty function for comparing two options given as
     *       Node pointers.
     *
     * status: DONE
     *
     * COMMENT:  depends on public compare(double,double,double,double) being implemented.
     *           You might find this version handy when manipulating lists
     */
    static Relationship compare(Node *a, Node *b) {
        if(a==nullptr || b==nullptr) {
            std::cout << "ERR: compare(Node*,Node*);  null pointer passed!!! Whoops!" << std::endl;
            return incomparable;
        }
        return compare(a->price, a->time, b->price, b->time);
    }
    
public:
    
    
    
    
    
    /**
     * function: push_front
     * description: Adds a <price,time> option to the front of the list (simple primitive for building lists)
     * status:  DONE
     */
    void push_front(double price, double time) {
        
        front = new Node(price, time, front);
        _size++;
    }
    
    /**
     * function: from_vec
     * description: This function accepts a C++ standard libary vector of pair<double,double>.
     *       Each pair is interpreted as a <price,time> option and a TravelOptions object
     *       is containing exactly the same options as in the vector (and in the same order).
     *
     * returns: a pointer to the resulting TravelOptions object
     * status:  DONE
     */
    static TravelOptions * from_vec(std::vector<std::pair<double, double> > &vec) {
        TravelOptions *options = new TravelOptions();
        
        for(int i=vec.size()-1; i>=0; i--) {// CHECK THIS ISSUE OUT, MAYBE ITS BECAUSE OF XCODE,
            // MIGHT WORK WELL ON BERT
            options->push_front(vec[i].first, vec[i].second);
        }
        return options;
    }
    
    /**
     * function: to_vec
     * description: Utility function which creates a C++ standard libary vector of pair<double,double>.
     *       and populates it with the options in the calling object (in the same order).
     *       As in from_vec the "first" field of each pair maps to price and the "second"
     *         field maps to time.
     *
     * returns: a pointer to the resulting vector
     * status:  DONE
     */
    std::vector<std::pair<double, double>> * to_vec() const {
        std::vector<std::pair<double, double>> *vec = new std::vector<std::pair<double, double>>();
        Node *p = front;
        
        while(p != nullptr) {
            vec->push_back(std::pair<double,double>(p->price, p->time));
            p = p->next;
        }
        return vec;
    }
    
    
    /**
     * func: is_sorted
     * desc: we consider an option list sorted under the following conditions:
     *
     *        - the options are in non-decreasing order of price AND
     *        - time is used as a tie-breaker for options with identical price.
     *
     *       For example, using the notation <price, time> to represent an option:
     *
     *             <5.1, 10.0>  must be before <5.6, 9.0>    (price is less, so time ignored)
     *             <6.2, 4.1>   must be AFTER  <6.2, 3.9>    (identical price; tie broken by
     *                                                         smaller time (3.9 in this case)).
     *
     *       If two or more options are identical in BOTH price and time, they are
     *       indistinguishible and must appear as a consecutive "block" if the list is
     *       to be considered sorted.
     *
     * returns:  true if sorted by the rules above; false otherwise.
     *
     * Examples:
     *
     *   The option list below is sorted by our rules:
     *      [ <1, 7>, <2, 8>, <2, 9>, <3, 5>, <5, 8>, <5, 8>, <5, 9>, <6, 12> ]
     *
     *   The option list below is NOT sorted by our rules:
     *      [ <1, 7>, <2, 8>, <4, 3>, <3, 7>]
     *                                ^^^^^^ must be before <4,3>
     *
     *   The option list below is also NOT sorted by our rules:
     *      [ <1, 7>, <2, 8>, <2, 5>, <3, 7>]
     *                        ^^^^^^ must be before <2,8>
     * status: DONE
     */
    bool is_sorted()const{
        
        Node *p;
        p = front;

        bool is_dup = true;
        // Traverses the entire list
        while (p -> next != nullptr){
            // If prices are more, then it returns false
            if ((p -> price) > (p -> next -> price)){
                is_dup = false;
                break;
            }
            // If prices are the same, then it compares time
            if ((p -> price) == (p -> next -> price)){
                if ((p -> time) > (p -> next -> time)){
                    is_dup = false;
                    break;
                }
            }
            p = p -> next; // Advances to the next node
        }
        return is_dup;

        return true;
    }
    
    
    //----------------------------------------------------------------------------------------------------------
    /**
     * function: is_pareto
     * description: returns true if and only if:
     *
     *        all options are distinct  (no duplicates)  AND
     *        none of the options are 'suboptimal' (i.e., for each option X, there DOES NOT EXIST
     *           any other option Y such that Y dominates X).  There are several equivalent
     *           ways of stating this property...
     *
     * status: DONE
     *
     * REQUIREMENTS:
     *    - the list must be unaltered
     *    - no memory allocation, arrays, etc. allowed
     *    - RUNTIME:  quadratic in number of options n (i.e., O(n^2)).
     *
     * REMEMBER:  the list does not need to be sorted in order to be pareto
     */
    bool is_pareto() const{
        
        Node *List1;
        List1 = front;
        Node *List2;
        List2 = front;
        
        //counter for dup check
        List2 = List1->next;
        while (List1 -> next != nullptr) {
            while (List2 != nullptr){
                if (List1 -> price == List2 -> price){
                    return false;
                }
                if ((List1 -> price > List2 -> price) && (List1 -> time > List2 -> time)){
                    return false;
                }
                // if List 2 price and time is less than List 1 price and time
                if ((List1 -> price < List2 -> price) && (List1 -> time < List2 -> time)){
                    return false;
                }
                List2 = List2 -> next;
            }
            List1 = List1 -> next;
            List2 = List1 -> next;
        }
        
        return true;
        
    }
    //----------------------------------------------------------------------------------------------------------
    /**
     * function: is_pareto_sorted()
     * description: returns true if and only if the list is:
     *    - STRICTLY INCREASING IN price AND
     *    - STRICTLY DECREASING IN time
     *
     * REQUIREMENTS:
     *   RUNTIME:  linear in length of list n (i.e., O(n)).
     *
     * status:  DONE
     *
     * COMMENTS:  notice that because of the runtime requirement, you cannot simply do this:
     *
     return is_sorted() && is_pareto();
     
     */
    
    // Function works solidly
    bool is_pareto_sorted() const{
        
        Node *p = front;
        bool is_sorted = true ;
        
        if(!(front == nullptr))
        {
            while (p -> next != nullptr){
                if (((p -> price) < (p -> next -> price)) && (p->time > p->next->time)){
                    p = p -> next;
                    continue;
                }
                is_sorted = false;
                break;
            }
            cout<<"is par sorted "<<is_sorted;
        }
        return is_sorted;
        
    }
    
    //-----------------------------------------------------------------------------------------------------------
    /**
     * func: insert_sorted
     * preconditions:  given collection (calling object) must be sorted (but not necessarily
     *                   pareto).  If this is not the case, false is returned (code provided).
     *                   (returns true otherwise, after insertion complete).
     *
     * desc:  inserts option <price,time> (given as parameters) into option list (calling object)
     *          while keeping it sorted.  Recall:  ordering by price; tie-breaker is time.
     *
     * RUNTIME:  linear in length of list -- O(n).
     *
     * status: DONE
     *
     */
    
    // Check this function
    
    bool insert_sorted(double price, double time) {
        if(!is_sorted()) return false; // checks if the calling object is sorted
        
        Node *p = front;
        Node *temp = new Node();
        Node *newNode = new Node(price, time);
        
        temp -> price = price;
        temp -> time = time;
        
        while (p -> next != nullptr){
            if ((p -> price < price && p -> next -> price > price) ||
                (p->price == price && p->time < time && p->next->price != price))
            {
                temp = p -> next;
                p-> next = newNode;
                newNode->next = temp;
                break;
            }
            else if(p->price == price && p->time > time)
            {
                temp = new Node(p->price, p->time, p->next);
                p->price = newNode->price;
                p->time = newNode->time;
                p->next = temp;
                
                break;
            }
            else
            {
                p = p->next;
            }
            
        }
        if (p -> next == nullptr){
            if ((p -> price < price) || (p->price == price && p->time > time)){
                p -> next =  newNode;
            }
            else{
                temp = new Node(p->price,p->time);
                p -> price = newNode -> price;
                p -> time =  newNode -> time;
                p -> next = new Node(temp->price,temp->time);
            }
        }
        return true;
    }
    
    //-----------------------------------------------------------------------------------------------------------
    
    /**
     * function: insert_pareto_sorted
     * preconditions:  given collection (calling object) must be sorted AND pareto (pruned).
     *                 if this is not the case, false is returned.
     *                 (code segment for this test given).
     * description: (assuming the list is sorted and pareto): if the option given by the parameters
     *       price and time is NOT dominated by already existing options, the following results:
     *            - new option <price,time> is inserted maintaining the sorted property of the
     *                list, AND
     *            - any pre-existing options which are now suboptimal (i.e., dominated by the
     *                newly added option) are deleted.
     *       If the new option is suboptimal, the list is simply unchanged.
     *       In either case, true is returned (i.e., as long as the preconditions are met).
     *
     * RUNTIME REQUIREMENT:  linear in the length of the list -- O(n)
     *
     * REMEMBER:
     *    If the new option is useless (dominated by a pre-existing option), the list is unchanged
     *         (but you still return true if preconditions are met).
     *    You must maintain sorted order and don't forget to deallocate memory associated
     *         with any deleted nodes.
     * status: DONE
     */
    bool insert_pareto_sorted(double price, double time) {
        if(!is_pareto_sorted()) return false;
        
        Node *p = front;
        Node *newNode = new Node;
        //Node *p = front; // to store pointer
        Node *temp2 = new Node;
        // adding the values to the new node
        newNode -> price = price;
        newNode -> time = time;
        
        // if the list is empty
        if (p == nullptr){
            p = newNode;
            p -> next = nullptr;
            front = p;
            return true;
        }
        
        if(p->next == nullptr){
            if (p -> price == newNode -> price && p -> time > newNode -> time){
                newNode -> next = p -> next;
                p = newNode;
            }
            else{
                p->next = newNode;
            }
        }
        
        while (p -> next != nullptr){
            if (p -> price < newNode -> price && p -> next -> price < newNode -> price){
                p = p -> next;
                continue;
            }
            else if((p -> next -> price >= newNode -> price) && (p -> next -> time > newNode -> time)){
                newNode -> next = p -> next -> next;
                delete p -> next;
                p -> next = newNode;
                p = p -> next;
                break;
            }
            else {
                break;
            }
        }
        // inserting at the end of the list?
        if (p -> next == nullptr){
            if (p -> price <= newNode -> price && p -> time >= time)
                p -> next = newNode;
        }
        // going through the list after inserting the node
        while (p -> next != nullptr){
            if (p -> price < p -> next -> price && p -> time > p -> next -> time){
                p = p -> next;
                continue;
            }
            if (p -> time < p -> next -> time){
                if (p -> next -> next != nullptr){ // if there are than 2 nodes ahead
                    temp2 -> next = p -> next -> next;
                    p->next = nullptr;
                    delete p -> next;
                    p->next = temp2 -> next;
                    continue;
                }
                if (p -> next -> next == nullptr){ // if there is only a single node ahead
                    temp2 -> next = p -> next -> next;
                    p->next = nullptr;
                    delete p -> next;
                    break;
                }
                continue;
            }
            
        }
        return true;
    }
    
    //------------------------------------------------------------------------------------------------------------
    
    /**
     * function: union_pareto_sorted
     * precondition:  calling object and parameter collections must both be sorted and pareto (if not, nullptr is returned).
     * description: constructs "the sorted, pareto" union (as in set-union excluding dominated options) of the two collections and returns
     *               it as a newly created object.
     * RUNTIME:  linear in the length of the two lists (suppose one list is of length n and the other is of length m,
     *            then the overall runtime must be O(n+m))
     * COMMENTS:  REMEMBER:  after this operation, the collection must be both pareto and sorted.
     * TIPS:  Think about what must be the FIRST option in the sorted union (bootstrap) -- then think about what is the
     *        candidate for the 2nd option (if any).
     *        Remember:  a pareto-sorted list must be strictly increasing and price and strictly decreasing in time.
     *
     * status:  DONE
     *
     */
    TravelOptions * union_pareto_sorted(const TravelOptions &other)const{
        if(!is_pareto_sorted() || !other.is_pareto_sorted()){
            return nullptr;
        }
        
        Node *list1 = this->front; // list A
        Node *list2 = other.front; // list B
        TravelOptions *list = new TravelOptions(); // new merged list
        Node *ptr = nullptr; // points to current node in new list
        list->front = ptr;
        Node *temp = nullptr;    // points to next node
        
        TravelOptions::Relationship r; // compares nodes from both lists and determines relationship
        
        // the other list is empty
        if(list1 == nullptr){
            list->front = list2;
            return list;
        }
        if(list2 == nullptr){
            list->front = list1;
            return list;
        }
        
        while(list1 != nullptr && list2 != nullptr){
            r = TravelOptions::compare(list1->price, list1->time, list2->price, list2->time);
            
            // nodes are identical
            if(r == TravelOptions::equal || r == TravelOptions::better){
                temp = list1;
                list1 = list1->next;
                temp->next = nullptr;
                
                //delete duplicate node in list B
                Node *toDelete = list2;
                list2 = list2->next;
                toDelete->next = nullptr;
                delete toDelete;
            }
            // node A is more expensive and slower than node B, dominated by tempB node
            else if(r == TravelOptions::worse){
                temp = list2;
                list2 = list2->next;
                temp->next = nullptr;
                
                //delete duplicate node in list A
                Node *toDelete = list1;
                list1 = list1->next;
                toDelete->next = nullptr;
                delete toDelete;
            }
            // nodes are incomparable
            else if (r == TravelOptions::incomparable){
                // list1  is cheaper than list 2
                if(list1->price < list2->price){
                    temp = list1;
                    list1 = list1->next;
                    temp->next = nullptr;
                }
                else if(list1->price > list2->price){
                    temp = list2;
                    list2 = list2->next;
                    temp->next = nullptr;
                }
            }
            if(ptr != nullptr){
                ptr->next = temp;
                ptr = ptr->next;
            }
            else{
                ptr = temp;
                if(list->front == nullptr){
                    list->front = ptr;
                }
            }
        }
        
        // done with list 1
        if(list1 == nullptr){
            temp->next = list2;
            ptr = temp;
            return list;
        }
        // done with list 1
        if(list2 == nullptr){
            temp->next = list1;
            ptr = temp;
            return list;
        }
        
        return list;
    }
    
    //------------------------------------------------------------------------------------------------------------
    
    /**
     * function:  prune_sorted
     * precondition:  given collection must be sorted (if not, false is returned).
     * description: takes sorted list of options and removes dominated entries
     *         (and eliminates any duplicates).
     * RUNTIME:  linear in the length of the list (O(n))
     * COMMENTS:  the resulting list will be sorted AND pareto.
     * status:  DONE
     *
     */
    bool prune_sorted(){
        if(!is_sorted()) return false;
        Node *p = front;
        Node *temp = nullptr; // to store the temporary pointer
        
        while (p -> next != nullptr){
            // Ideal situation where price is strictly decreasing and time is increasing
            if (p -> price < p -> next -> price && p -> time > p -> next -> time){
                p = p -> next;
                continue;
            }
            // if price matches, then time will play the deciding factor and delete the option
            if (p -> price <= p -> next -> price && p -> next -> next != nullptr){
                if (p -> time <= p -> next -> time){ // changed "<" to "<="
                    // I would want to delete the next node
                    temp = p -> next -> next; // changed "temp -> next to temp"
                    delete p -> next;
                    p -> next = temp -> next; // can it also be temp?
                    continue;
                }
            }
            if (p -> next -> next == nullptr){
                temp = nullptr;
                delete p -> next;
                p -> next = temp;
            }
            // two options have the exact same thing, like <5,8> <5,8>, we would want to delete one
            // and keep the other
            else{
                return true;
            }
        }
        return true;
    }
    
    
    /**
     * function: join_plus_plus
     * preconditions:  none -- both the calling object and parameter are just TravelOptions objects (not necessarily
     *                 sorted or pareto).
     * parameters: other; a reference to another to a list of TravelOptions (thus, there are two option lists: the calling
     *               object and the parameter).
     * description: suppose you are planning a trip composed of two "legs":
     *
     *         first you travel from city X to city Y (this is part of your plan - you need to stop in city Y)
     *         then you continue from city Y to city Z
     *
     *       Let the calling object be the options for the X-to-Y leg and the parameter be the options for the
     *       Y-to-Z leg.
     *
     *       Your job is to construct a pareto-sorted set of options for the entire trip and return the resulting
     *       TravelOptions object as a pointer.
     *
     *       In principle, any option <p1,t1> for the X-to-Y leg can be paird with any option <p2,t2> for the Y-to-Z leg.
     *       In such a pairing, the total price will be p1+p2 and the total time will be t1+t2, (i.e., option <p1+p2,t1+t2>).
     *       (This is why the operation is called join_plus_plus)
     *
     *       This function's  job is to determine the sorted-pareto list of options for the entire trip and return it
     *       as a pointer to the object.
     * returns:  a pointer to a TravelOptions object capturing all non-dominated options for the entire trip from X-to-Z
     *              (i.e., even though the given lists may not be sorted or pareto, the resulting list will be both).
     *
     * status:  DONE
     * RUNTIME:  no runtime requirement
     *
     */
    TravelOptions * join_plus_plus(const TravelOptions &other) const {
        
        Node *list1 = front;
        Node *list2 = other.front;
        TravelOptions *t = new TravelOptions();
        // if lists are empty return t
        if(list1 != nullptr && list2 != nullptr)
        {
            while(list1!= nullptr)
            {
                list2 = other.front;
                while(list2!= nullptr)
                {
                    double price = list1->price+list2->price;
                    double time = list1->time+list2->time;
                    t->insert_pareto_sorted(price, time);
                    
                    list2 = list2->next;
                }
                
                list1 = list1->next;
            }
        }
        
        return t;  // placeholder to make the compiler happy
    }
    
    
    
    /**
     * function: join_plus_max
     * preconditions:  both the calling object and the parameter are sorted-pareto lists (if not, nullptr is returned).
     * description: imagine a different scenario (vs. join_plus_plus):  you are a parent with two children -- one living in city A and
     *         the other living in city C -- and they are both coming home for the holidays (let's call your home C).
     *       You have a pareto-sorted option list for the A-to-C trip (this is the calling object) and a pareto-sorted option list
     *         for the B-to-C trip.
     *       Consider a particular option for the A-to-C trip <p1, t1> and a particular option for the B-to-C trip <p2,t2>.
     *         Clearly, the total price for selecting these two options is p1+p2.
     *         However (unlike in the plus_plus case), adding the t1 and t2 doesn't make sense right (the trips are in "parallel").
     *         What you care about as a parent is when BOTH of your children are home.  This is determine by MAX(t1,t2).
     *         Thus, the resulting "composite" option in this case would be <p1+p2, MAX(t1,t2)> (hence the name join_plus_max).
     *
     *       Your job is to construct the sorted-pareto "composite" option list capturing the options for getting both children home.
     *         The resulting TravelOptions object is returned as a pointer (recall if the preconditions are not met,
     *         nullptr is returned).
     *
     * RUNTIME:  let N and M be the lengths of the respective lists given; your runtime must be linear in N+M (O(N+M)).
     *
     * status:  TODO
     *
     */
    
    TravelOptions * join_plus_max(const TravelOptions &other) const {
        
        if(!is_pareto() || !(other.is_pareto()))
            return nullptr;
        
        TravelOptions *t = new TravelOptions();
        Node *list1 = front;
        Node *list2 = other.front;
        Node *temp = t->front;
        
        while(list1 != nullptr && list2 != nullptr)
        {
            double p1=0,p2=0,p3=0,t1,t2,t3;
            
            p1 = list1->price+list2->price;
            t1 = max(list1->time,list2->time);
            appendNode(t->front,p1,t1);
            
            if(list2->next != nullptr)
            {
                p2 = list1->price+list2->next->price;
                t2 = max(list1->time,list2->next->time);
            }
            
            if(list1->next != nullptr)
            {
                p3 = list1->next->price+list2->price;
                t3 = max(list1->next->time,list2->time);
                
            }
            
            if(p2!=0 && p2<p3)
                appendNode(t->front,p2,t2);
            
            if(p3!=0 && p3<p2)
                appendNode(t->front,p3,t3);
            
            if(p2!=0 && p2>p3)
                appendNode(t->front,p2,t2)
                ;
            if(p3!=0 && p3>p2)
                appendNode(t->front,p3,t3)
                ;
            
            list1 = list1->next;
            list2 = list2->next;
        }
        
        t->prune_sorted();
        return t;
    }
    
    Node *findLastNodeOnList (Node *pTemp)const{
        while (pTemp != nullptr && pTemp -> next != nullptr){
            pTemp = pTemp -> next;
        }
        return pTemp;
    }
    //--------------------------------------------
    void appendNode (Node *&pHead, double price, double time)const{
        Node *pTemp = new Node;
        pTemp -> price = price;
        pTemp->time = time;
        pTemp -> next = nullptr;
        Node *pEndOfList = findLastNodeOnList(pHead);
        if (pEndOfList == nullptr){
            pHead = pTemp;
        }
        else{
            pEndOfList -> next = pTemp;
        }
    }
    
    /**
     * function: sorted_clone
     * description: returns a sorted TravelOptions object which contains the same elements as the current object
     * status:  DONE [but relies on to do item insert_sorted]
     */
    TravelOptions * sorted_clone() {
        TravelOptions *sorted = new TravelOptions();
        Node *p = front;
        
        while(p != nullptr) {
            sorted->insert_sorted(p->price, p->time);
            p = p->next;
        }
        return sorted;
    }
    
    /**
     * function: split_sorted_pareto
     * precondition:  given list must be both sorted and pareto (if not, nullptr is returned;
     *    code already given).
     * description: This function takes a sorted-pareto option list and splits into two option lists:
     *
     *        - the options with price less than or equal to max_price (if any) are retained in the calling
     *            object (and only those are retained in the calling object).
     *        - the other, more expensive options are used to populate a new TravelOptions object which
     *            is returned as a pointer (i.e., the expensive options are returned)..
     *
     * returns: pointer to expensive options or nullptr if the calling object is not pareto-sorted.
     * RUNTIME:  linear in the length of the given list (O(n)).
     * ADDITIONAL REQUIREMENT:  for full credit you MAY NOT ALLOCATE ANY NEW NODES!  Think about it --
     *        suppose your given list has 100 options and 40 of them are below the max_price threshold;
     *        the other 60 options end up in the returnd list.  Still a grand total of 100 options and
     *        therefore 100 nodes.  So... there should be no reason to delete or allocate any nodes.
     * status:  TODO
     */
    TravelOptions * split_sorted_pareto(double max_price) {
        
        if(!is_pareto_sorted())
            return nullptr;
        
        TravelOptions *t = new TravelOptions();
        Node *p = this->front;
        while(p->next != nullptr)
        {
            if(p->price == max_price)
            {
                t->front = p->next;
                p->next = nullptr;
                break;
            }
            p = p->next;
        }
        return t;  // placeholder to make compiler happy with skeleton
        
    }
    
    /**
     * func: display
     * desc: prints a string representation of the current TravelOptions object
     * status:  DONE
     */
    void display(){
        printf("   PRICE      TIME\n");
        printf("---------------------\n");
        Node * p = front;
        
        while(p!=nullptr) {
            printf("   %5.2f      %5.2f\n", p->price, p->time);
            p = p->next;
        }
    }
    
    /**
     * func:  checksum
     * desc:  Performs and XOR of all node pointers and returns result as
     *        an unsigned int.
     *
     * status: DONE
     *
     * NOTES: YOU MAY NOT TOUCH OR MODIFY THIS FUNCTION!!
     */
    unsigned long int checksum() const {
        unsigned long int s = 0;
        Node *p = front;
        
        while (p != nullptr)
        {
            s = s ^ ((unsigned long int)p);
            p = p->next;
        }
        return s;
    }
    
};

//#endif
