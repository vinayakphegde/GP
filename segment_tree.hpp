#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <iostream>
#include <iterator>
#include <typeinfo>
using namespace std;

#define mid(l,r) (l+(r-l)/2)

#include "iterator.hpp"
using namespace seg_tree;

#include "operation.hpp"

template<typename T,typename pred_t>
class segment_tree
{
	private:
	typedef struct tree_node{
		T value;
		tree_node* ptr;
	}tree_node;
	vector<tree_node> tree;
	// vector<T> __tree;
	vector<T> lazy;
	int n;
	int size_;

	// template<typename iter>
	// T find_ele(iter &begin_, iter &end_)
	// {
	// 	if(begin_!=end_)
	// 		return *begin_++;

	// 	cout<<"WARNING: Returning out of range!";
	// 	return *end_;
	// }
	template<typename iter>
	T find_ele(iter begin, int index, input_iterator_tag)
	{
		while(index > 0)
		{
			begin++;
			index--;
		}
		return *begin;
	}
	template<typename iter>
	T find_ele(iter begin, int index, bidirectional_iterator_tag )
	{
		return find_ele(begin, index, std::input_iterator_tag{});
	}
	template<typename iter>
	T find_ele(iter begin, int index, random_access_iterator_tag)
	{
		return *(begin + index);
	}

	template<typename iter>
	void build_tree(iter &begin_, iter &end_, int start,int end, int np)
	{
		if(end == start)
		{
			tree[np].value = find_ele(begin_, start, typename iterator_traits<iter>::iterator_category());
			tree[np].ptr = &tree[np];
			// __tree[np] = *tree[np];
			// tree[np] = find_ele(begin_,end_);
			return;
		}
		int mid=mid(start,end);
		build_tree(begin_, end_, start,mid,2*np+1);
		build_tree(begin_, end_, mid+1,end,2*np+2);
		tree[np] = pred_t()(tree[2*np+1],tree[2*np+2]);
		// __tree[np] = *tree[np];
		
	}
	
	tree_node query_util(int start,int end,int query_start,int query_end,int np) const
	{
		if(start>query_end or end<query_start)
		{
			tree_node x = {*pred_t().n_element,nullptr};
			return x; 
		}
		if(query_start<=start and query_end>=end)
		{
			return tree[np];
		}
		int mid=mid(start,end);
		tree_node left=query_util(start,mid,query_start,query_end,2*np+1);
		tree_node right=query_util(mid+1,end,query_start,query_end,2*np+2);
		return pred_t()(left,right);
	}
	
	void update_util(int start,int end,int index,int value,int np)
	{
		if(index<start or index>end)
			return;
		
		if(start==end)
		{
			*tree[np]=value;
			return;
		}
		int mid=mid(start,end);
		if(index>mid)
		{
			update_util(mid+1,end,index,value,2*np+2);
		}
		else
		{
			update_util(start,mid,index,value,2*np+1);
		}

		tree[np]=pred_t()(tree[2*np+1],tree[2*np+2]);
	}
	
	void updateRange_util(int start,int end,int query_start,int query_end,int value,int np){
		if (lazy[np] != 0)
    	{
	        *tree[np] += lazy[np];
			// __tree[np] = *tree[np];
        	if (start != end)
    		{
            	lazy[np*2 + 1]   += lazy[np];
            	lazy[np*2 + 2]   += lazy[np];
  	    	}
        	lazy[np] = 0;
    	}
		if(start>query_end or end<query_start)
			return;
		if (start>=query_start && end<=query_end)
    	{
        	*tree[np] += value;
			// __tree[np] = *tree[np];
        	if (start != end)
        	{
            	lazy[np*2 + 1]   += value;
            	lazy[np*2 + 2]   += value;
        	}
        	return;
    	}
		int mid=mid(start,end);
		updateRange_util(start,mid,query_start,query_end,value,2*np+1);
		updateRange_util(mid+1,end,query_start,query_end,value,2*np+2);
   		tree[np]=pred_t()(tree[2*np+1],tree[2*np+2]);	
		// __tree[np] = *tree[np];
	}




	T* get_neutral_ele()
	{
		return pred_t().n_element;
	}

	public:
	// Default Constructor
	segment_tree() = delete;

	// Parameterized Constructor
	template<typename iter>
	segment_tree(iter begin, iter end)
	{
		size_ = (int)distance(begin, end);
		n = 1;
		while(n < size_){
			n = n*2;
		}
		n = n*2;

		tree.resize(n);
		lazy.resize(n,0);
		// __tree.resize(n,*get_neutral_ele());
		for(int i=0;i<n;++i)
		{
			tree[i] = {*get_neutral_ele() , nullptr};
		}
		build_tree(begin, end, 0, size_-1, 0);
	}
	
	// Copy Constructor - TODO
	segment_tree(Iterator<T> begin, Iterator<T> end)
	{
		// DO SOMETHING THAT WORKS
	}
	
	// Destructor
	~segment_tree()
	{
		// DO NOTHING
	}
	
	
	Iterator<tree_node> query(int query_start,int query_end) const
	{
		return Iterator<tree_node>(&query_util(0,size_-1,query_start,query_end,0));
	}
	void update(int index,int value)
	{
		return update_util(0,size_-1,index,value,0);
	}

	void updateRange(int query_start,int query_end,int value){
		return updateRange_util(0,size_-1,query_start,query_end,value,0);
	}
	void display()
	{
		for(int i=0;i<n;i++)
		{
			cout << *tree[i] <<"  "; 
		}
		cout << "\n";
	}
	int size() const
	{
		return size_;
	}
	bool empty() const
	{
		return size==0;
	}
	Iterator<T> begin()
	{
		return Iterator<T>(tree.begin());
	}
	Iterator<T> end()
	{
		return Iterator<T>(tree.end());
	}
	friend ostream& operator<<(ostream& O, Iterator<tree_node> node){
		O << node.value ;
		return O;
	}
};

template<typename T>
class segment_tree<T, operation::sum<T> >
{
	private:
	vector<T> tree;
	vector<T> lazy;
	int n;
	int size_;

	// template<typename iter>
	// T find_ele(iter &begin_, iter &end_)
	// {
	// 	if(begin_!=end_)
	// 		return *begin_++;
	// 	cout<<"WARNING: Returning out of range!";
	// 	return *end_;
	// }
	template<typename iter>
	T find_ele(iter begin, int index, input_iterator_tag)
	{
		while(index > 0)
		{
			begin++;
			index--;
		}
		return *begin;
	}
	template<typename iter>
	T find_ele(iter begin, int index, bidirectional_iterator_tag )
	{
		return find_ele(begin, index, std::input_iterator_tag{});
	}
	template<typename iter>
	T find_ele(iter begin, int index, random_access_iterator_tag)
	{
		return *(begin + index);
	}

	template<typename iter>
	void build_tree(iter &begin_, iter &end_, int start,int end, int np)
	{
		if(end == start)
		{
			tree[np] = find_ele(begin_, start, typename iterator_traits<iter>::iterator_category());
			// tree[np] = find_ele(begin_,end_);
			return;
		}
		int mid=mid(start,end);
		build_tree(begin_, end_, start,mid,2*np+1);
		build_tree(begin_, end_, mid+1,end,2*np+2);

		tree[np]=operation::sum<T>()(tree[2*np+1],tree[2*np+2]);
	}
	
	T query_util(int start,int end,int query_start,int query_end,int np) 
	{

		if (lazy[np] != 0)
    	{
        	tree[np] += (end-start+1)*lazy[np];
       		if (start != end)
        	{
            	lazy[np*2+1] += lazy[np];
            	lazy[np*2+2] += lazy[np];
        	}
  
        	lazy[np] = 0;
  		}
		if(start>query_end or end<query_start)
		{
			return operation::sum<T>().n_element;
		}
		if(query_start<=start and query_end>=end)
		{
			return tree[np];
		}
		int mid=mid(start,end);
		int left=query_util(start,mid,query_start,query_end,2*np+1);
		int right=query_util(mid+1,end,query_start,query_end,2*np+2);
		return operation::sum<T>()(left,right);
	}
	
	void update_util(int start,int end,int index,int value,int np)
	{
		if(index<start or index>end)
			return;
		
		if(start==end)
		{
			tree[np]=value;
			return;
		}
		int mid=mid(start,end);
		if(index>mid)
		{
			update_util(mid+1,end,index,value,2*np+2);
		}
		else
		{
			update_util(start,mid,index,value,2*np+1);
		}

		tree[np]=operation::sum<T>()(tree[2*np+1],tree[2*np+2]);
	}

	void updateRange_util(int start,int end,int query_start,int query_end,int value,int np){
		if (lazy[np] != 0)
    	{
	        tree[np] += (end-start+1)*lazy[np];
        	if (start != end)
    		{
            	lazy[np*2 + 1]   += lazy[np];
            	lazy[np*2 + 2]   += lazy[np];
  	    	}
        	lazy[np] = 0;
    	}
		if(start>query_end or end<query_start)
			return;
		if (start>=query_start && end<=query_end)
    	{
        	tree[np] += (end-start+1)*value;
  
        	if (start != end)
        	{
            	lazy[np*2 + 1]   += value;
            	lazy[np*2 + 2]   += value;
        	}
        	return;
    	}
		int mid=mid(start,end);
		updateRange_util(start,mid,query_start,query_end,value,2*np+1);
		updateRange_util(mid+1,end,query_start,query_end,value,2*np+2);
   		tree[np]=operation::sum<T>()(tree[2*np+1],tree[2*np+2]);
	}
	
	int get_neutral_ele()
	{
		return operation::sum<T>().n_element;
	}

	public:
	// Default Constructor
	segment_tree() = delete;

	// Parameterized Constructor
	template<typename iter>
	segment_tree(iter begin, iter end)
	{
		cout<<"Specialized class\n";
		size_ =  (int)distance(begin, end);
		n = 1;
		while(n < size_){
			n = n*2;
		}
		n = n*2;

		tree.resize(n,get_neutral_ele());
		lazy.resize(n,get_neutral_ele());
		build_tree(begin, end, 0, size_-1, 0);
	}



	// Copy Constructor - TODO
	segment_tree(const segment_tree<T,operation::sum<T> >& other): size_(other.size_), n(other.n)
	{
		// DO SOMETHING THAT WORKS
	}
	
	// Destructor
	~segment_tree()
	{
		// DO NOTHING
	}
	
	
	T query(int query_start,int query_end) 
	{
		return query_util(0,size_-1,query_start,query_end,0);
	}
	void update(int index,int value)
	{
		return update_util(0,size_-1,index,value,0);
	}

	//range update
	void updateRange(int query_start,int query_end,int value){
		return updateRange_util(0,size_-1,query_start,query_end,value,0);
	}


	T operator[](int index) const
	{
		return query(index,index);
	}
	void display()
	{
		for(int i=0;i<n;i++)
		{
			cout << tree[i] <<"  "; 
		}
		cout << "\n";
	}
	int size() const
	{
		return size_;
	}
	bool empty()
	{
		return size==0;
	}
};

#endif