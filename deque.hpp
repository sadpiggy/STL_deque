#ifndef SJTU_DEQUE_HPP//shadow die twice==delete twice==return code 139==wo ri TMD
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
const int block_max_size=300;
const int block_split_remain=150;
const int merge_min=120;
namespace sjtu {

    template<class T>
    class deque {
    public:
        class node{
        public:
            T* value= nullptr;
            node* pre_node= nullptr;
            node* next_node= nullptr;

            node()=delete;

            node(const node& other){
                pre_node= nullptr;
                next_node= nullptr;
                T zhi=*other.value;
                value=new T(zhi);
            }

            node(node* pre_,node* next_,const T& value_):pre_node(pre_),next_node(next_){value=new T(value_);}//可能会memory le

           explicit node(const T& value_){value=new T(value_);}//just use to test

           //node(T* va){value=new T(*va);}

            node& operator=(const node& othr)=delete;

            ~node(){
                if(value!= nullptr)delete value;
            }
        };

        class block{
        public:
            int nodes_num=0;
            node*  head_node= nullptr;
            node*  tail_node= nullptr;
            block* pre_block= nullptr;
            block* next_block= nullptr;

            block()=delete;

            block(const int num_,node* head_,node* tail_,block* pre_,block* next_):nodes_num(num_),head_node(head_),tail_node(tail_),pre_block(pre_),next_block(next_){}

            block( block& other){
                node* now_node=other.head_node;
                while(now_node!= nullptr)
                {
                    push_node_back(now_node);
                    now_node=now_node->next_node;
                }
            }

            void push_node_back( node* other)
            {
                if(tail_node!= nullptr){
                    tail_node->next_node=new node(*(other));
                    tail_node->next_node->pre_node=tail_node;
                    tail_node=tail_node->next_node;
                } else{
                    head_node=new node(*other);
                    tail_node=head_node;
                }
                nodes_num++;
            }


            ~block(){

               // std::cout<<"delete_"<<'\n';

                node* mid=head_node;int pig=0;


                for(pig=0;pig<nodes_num;pig++ )
                {
                    mid=head_node;
                    if(mid== nullptr)break;
                    head_node=head_node->next_node;
                    delete mid;
                }

                nodes_num=0;
            }

            block& operator=(const block& other)=delete;

            node* find_node(int x){//x>=0   fing_block
                node* mid=head_node;
                for(int i=0;i<x;i++)mid=mid->next_node;
                return mid;
            }

            block* split_block(deque<T>&deque_)
            {
               // std::cout<<"split"<<'\n'<<deque_.blocks_num<<'\n';//block(1,)  liekai
                if(deque_.blocks_num==2)du_duab_dian();
                block* new_block=new block(nodes_num-block_split_remain, nullptr, nullptr, this,next_block);
                nodes_num=block_split_remain;
                if(next_block!= nullptr){next_block->pre_block=new_block;/*std::cout<<"budengyu  NULL"<<deque_.blocks_num<<'\n';*/}
                next_block=new_block;
                node* new_head=find_node(block_split_remain);
                new_block->head_node=new_head;new_block->tail_node=tail_node;
                tail_node=new_head->pre_node;
                tail_node->next_node= nullptr;
                new_block->head_node->pre_node= nullptr;
                //std::cout<<"==daodile"<<deque_.length<<'\n';
                if(this==deque_.tail_block)deque_.tail_block=new_block;
                 deque_.blocks_num++;
                return new_block;
            }

            void du_duab_dian()
            {
              //  std::cout<<"xixixix"<<'\n';
            }

            void merge_block()//因为应用环境，下一块必不为空//且合并只能和下一块合并
            {

               // std::cout<<"merge"<<std::endl;

                /*if(nodes_num==0)//
                {
                    if(next_block== nullptr)
                    {

                    }

                    head_node=next_block->head_node;tail_node=next_block->tail_node;
                }*/

                //else
                  //  {
                    if(next_block->nodes_num!=0){
                        tail_node->next_node=next_block->head_node;
                        next_block->head_node->pre_node=tail_node;
                        tail_node=next_block->tail_node;
                    }
                //}
                nodes_num=nodes_num+next_block->nodes_num;
                if(next_block->next_block!= nullptr)next_block->next_block->pre_block= this;
                block* mid_block=next_block;
                next_block->head_node= nullptr;
                next_block->tail_node= nullptr;
                next_block=next_block->next_block;
                mid_block->nodes_num=0;
                delete mid_block;//delete
            }

            void insert_node(deque<T>&deque_, node* other,int offset_)
            {
               // std::cout<<"tututu";
                if(nodes_num==0){head_node=other;tail_node=other;}
                if(nodes_num!=0)
                {
                    bool flag_=false;
                    if(offset_==0)
                    {
                        //nodes_num++;
                        other->next_node=head_node;head_node->pre_node=other;
                        head_node=other;
                        flag_=true;
                    }
                    if(offset_==nodes_num)
                    {
                        //nodes_num++;
                        tail_node->next_node=other;other->pre_node=tail_node;
                        tail_node=other;
                        flag_=true;
                    }
                    if(flag_==false)
                    {
                        node* cur_node=find_node(offset_);
                        other->pre_node=cur_node->pre_node;
                        other->next_node=cur_node;
                        cur_node->pre_node->next_node=other;
                        cur_node->pre_node=other;
                    }
                }
                nodes_num++;
                deque_.length++;
               // std::cout<<"wo shi sb"<<nodes_num<<std::endl;throw
                if(nodes_num==block_max_size)//裂开
                {

                  //  if(this==deque_.tail_block)std::cout<<deque_.blocks_num<<"==block"<<std::endl;

                //    std::cout<<"nodes num=="<<nodes_num<<std::endl;

                   block* new_block= split_block(deque_);


                   //std::cout<<deque_.blocks_num<<"num"<<'\n';


                   //std::cout<<"liekai"<<std::endl;

                   //std::cout<<";iekuai"<<new_block->nodes_num<<"new";
                    //if(this==deque_.tail_block)deque_.tail_block=new_block;
                   // deque_.blocks_num++;
                }
                //
            }




            void delete_node(deque<T>&deque_,int offset_)//在下面用这个函数的时候，记得判断是否出界
            {
                if(nodes_num==0)throw container_is_empty();
                if(nodes_num==1){delete head_node;head_node= nullptr;tail_node= nullptr;}
                if(nodes_num!=0&&nodes_num!=1)
                {
                    if(offset_==0)
                    {
                        node* mid_node=head_node;
                        head_node=head_node->next_node;
                        head_node->pre_node= nullptr;
                        delete mid_node;
                    }

                    if(offset_>=nodes_num-1)
                    {
                        node* node_mid=tail_node;
                        tail_node=tail_node->pre_node;
                        tail_node->next_node= nullptr;
                        delete node_mid;
                    }

                    if(offset_>0&&offset_<(nodes_num-1))
                    {
                        node* mid=find_node(offset_);
                        mid->pre_node->next_node=mid->next_node;
                        mid->next_node->pre_node=mid->pre_node;
                        delete mid;
                    }
                }
                nodes_num--;
                deque_.length--;

                if(deque_.blocks_num==1)return;

                //块一定大于等于2

                if(nodes_num==0)
                {
                    if(this->next_block== nullptr){//此时，她的上一个一定存在
                        block* block_pre=this->pre_block;
                        deque_.tail_block=block_pre;
                        delete this;
                        block_pre->next_block= nullptr;
                        return;
                    }

                    if(this->pre_block== nullptr)
                    {
                        block* block_next=this->next_block;
                        deque_.head_block=block_next;
                        delete this;
                        block_next->pre_block= nullptr;
                        return;
                    }

                    block* block_pre=this->pre_block;
                    block* block_next=this->next_block;
                    delete this;
                    block_pre->next_block=block_next;
                    block_next->pre_block=block_pre;
                    return;
                }


                if(next_block== nullptr)return;

                if(nodes_num+next_block->nodes_num>merge_min)return;

                if(next_block==deque_.tail_block){
                    merge_block();
                    deque_.blocks_num--;
                    deque_.tail_block=this;
                    return;
                }

                merge_block();
                deque_.blocks_num--;
                //return;
            }





        };


    public:
        int length=0;
        int blocks_num=0;
        block* head_block= nullptr;
        block* tail_block= nullptr;

    public:
        class const_iterator;
        class iterator {

            friend class deque<T>;
        public:
            node* cur_node= nullptr;
            block* cur_block= nullptr;
            int cur_pos=-1;
            deque<T>*deque_ptr= nullptr;
            bool been_invalid=false;
            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:


            void set_invalid()
            {
                been_invalid=true;
            }
            /**
             * return a new iterator which pointer n-next elements
             *   if there are not enough elements, iterator becomes invalid
             * as well as operator-
             */
            //todo  my  destructor

            iterator()=default;

            iterator(node* node_ptr_,block* block_ptr_,int cur_pos_,deque<T>*deque_ptr_,bool invalid_=false){
                cur_node=node_ptr_;
                cur_block=block_ptr_;
                cur_pos=cur_pos_;
                deque_ptr=deque_ptr_;
                been_invalid=invalid_;
            }

            iterator(const iterator& other)
            {
                cur_node=other.cur_node;
                deque_ptr=other.deque_ptr;
                cur_block=other.cur_block;
                cur_pos=other.cur_pos;
                been_invalid=other.been_invalid;
            }

            iterator& operator=(const iterator& other)
            {
                if(this==&other)return *this;
                cur_node=other.cur_node;
                deque_ptr=other.deque_ptr;
                cur_block=other.cur_block;
                cur_pos=other.cur_pos;
                been_invalid=other.been_invalid;
                return *this;
            }


            iterator operator+(const int &n) const {
                //TODO
                if(cur_pos+n>deque_ptr->length||cur_pos+n<0)throw invalid_iterator();//throw index_out_of_bound()

              //  if(cur_pos+n>deque_ptr->length||cur_pos+n<0){
                //    set_invalid();
                    //been_invalid=true;
                  //  return *this;
                //}


                if(cur_pos+n==deque_ptr->length){
                    return iterator(nullptr, nullptr,deque_ptr->length,deque_ptr,been_invalid);
                }

                iterator iterator_mid;
                iterator_mid.deque_ptr=deque_ptr;
                iterator_mid.cur_pos=n+cur_pos;
                int offset_;
                iterator_mid.cur_block=deque_ptr->find_block(iterator_mid.cur_pos,offset_);
                block* block_mid=iterator_mid.cur_block;
                iterator_mid.cur_node=block_mid->find_node(offset_);
                return  iterator_mid;
            }
            iterator operator-(const int &n) const {
                //TODO
                if(cur_pos-n>deque_ptr->length||cur_pos-n<0)throw invalid_iterator();

              //  if(cur_pos-n>deque_ptr->length||cur_pos-n<0){
                //    set_invalid();
                  //  return *this;
                //}

                if(cur_pos-n==deque_ptr->length){
                    return iterator(nullptr, nullptr,deque_ptr->length,deque_ptr,been_invalid);
                }

                iterator iterator_mid;
                iterator_mid.deque_ptr=deque_ptr;
                iterator_mid.cur_pos=cur_pos-n;
                int offset_;
                iterator_mid.cur_block=deque_ptr->find_block(iterator_mid.cur_pos,offset_);
                block* block_mid=iterator_mid.cur_block;
                iterator_mid.cur_node=block_mid->find_node(offset_);
                return  iterator_mid;
            }
            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if(deque_ptr!=rhs.deque_ptr)throw invalid_iterator();
                return cur_pos-rhs.cur_pos;
            }
            iterator& operator+=(const int &n) {
                //TODO
             //   if(cur_pos+n>deque_ptr->length||cur_pos+n<0)throw invalid_iterator();
                if(cur_pos+n>deque_ptr->length||cur_pos+n<0){
                    set_invalid();
                    return *this;
                }
                cur_pos+=n;

                if(cur_pos==deque_ptr->length){
                    cur_block=deque_ptr->tail_block;
                    cur_node= nullptr;
                    return *this;
                }

                int offset_;
                cur_block=deque_ptr->find_block(cur_pos,offset_);
                cur_node=cur_block->find_node(offset_);
                return *this;
            }
            iterator& operator-=(const int &n) {
                //TODO
             //   if(cur_pos-n>deque_ptr->length||cur_pos-n<0)throw invalid_iterator();

                if(cur_pos-n>deque_ptr->length||cur_pos-n<0){
                    set_invalid();
                    return *this;
                }


                cur_pos-=n;int offset_;

                if(cur_pos==deque_ptr->length){
                    cur_block=deque_ptr->tail_block;
                    cur_node= nullptr;
                    return *this;
                }

                cur_block=deque_ptr->find_block(cur_pos,offset_);
                cur_node=cur_block->find_node(offset_);

                //*this=(*this)-n;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {//there may be some problem
                if(cur_pos==deque_ptr->length){
                    been_invalid=true;
                    return *this;
                }



                iterator ite_mid=*this;
                //*this=*this+1;
                *this+=1;
                return ite_mid;
                //ite_mid=*this+1;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                if(cur_pos==deque_ptr->length)
                {
                    set_invalid();
                    return *this;
                }
               // *this=*this+1;
                *this+=1;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {//报个寂寞的错

                if(cur_pos==0)
                {
                    set_invalid();
                    return *this;
                }

                iterator ite_mid=*this;
                //*this=*this-1;
                *this-=1;
                return ite_mid;
                //return *this;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {

                if(cur_pos==0)
                {
                    set_invalid();
                    return *this;
                }
                //*this=*this-1;
                *this-=1;
                return *this;
            }
            /**
             * TODO *it
             * 		throw if iterator is invalid//
             *///387column out of jie
            T& operator*() const {
                if(cur_pos<0||cur_pos>=deque_ptr->length||been_invalid)throw invalid_iterator();
                return *(cur_node->value);
            }
            /**
             * TODO it->field
             * 		throw if iterator is invalid
             */
            T* operator->() const noexcept {
                if(cur_pos<0||cur_pos>=deque_ptr->length||been_invalid)throw invalid_iterator();
                return (cur_node->value);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return false;
                return true;
            }
            bool operator==(const const_iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return false;
                return true;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return true;
                return false;
            }
            bool operator!=(const const_iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return true;
                return false;
            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
            node* cur_node= nullptr;
            block* cur_block= nullptr;
            int cur_pos=-1;
            const deque<T>*deque_ptr= nullptr;
            bool been_invalid=false;
        public:
            const_iterator() =default;
            const_iterator(const const_iterator &other) {
                cur_node=other.cur_node;
                deque_ptr=other.deque_ptr;
                cur_block=other.cur_block;
                cur_pos=other.cur_pos;
                been_invalid=other.been_invalid;
            }
            const_iterator(const iterator &other) {
                cur_node=other.cur_node;
                deque_ptr=other.deque_ptr;
                cur_block=other.cur_block;
                cur_pos=other.cur_pos;
                been_invalid=other.been_invalid;
            }

            const_iterator(node* node_ptr_,block* block_ptr_,int cur_pos_,const deque<T>*deque_ptr_,bool invalid_=false){
                cur_node=node_ptr_;
                cur_block=block_ptr_;
                cur_pos=cur_pos_;
                deque_ptr=deque_ptr_;
                been_invalid=invalid_;
            }


            const_iterator& operator=(const const_iterator& other)
            {
                if(this==&other)return *this;
                cur_node=other.cur_node;
                deque_ptr=other.deque_ptr;
                cur_block=other.cur_block;
                cur_pos=other.cur_pos;
                been_invalid=other.been_invalid;
                return *this;
            }

            const_iterator operator+(const int &n) const {
                //TODO
                if(cur_pos+n>deque_ptr->length||cur_pos+n<0)throw invalid_iterator();

                if(cur_pos+n==deque_ptr->length){
                    return const_iterator(nullptr, nullptr,deque_ptr->length,deque_ptr,been_invalid);
                }

                const_iterator iterator_mid;
                iterator_mid.deque_ptr=deque_ptr;
                iterator_mid.cur_pos=n+cur_pos;
                int offset_;
                iterator_mid.cur_block=deque_ptr->find_block(iterator_mid.cur_pos,offset_);
                block* block_mid=iterator_mid.cur_block;
                iterator_mid.cur_node=block_mid->find_node(offset_);
                return  iterator_mid;
            }
            const_iterator operator-(const int &n) const {
                //TODO
                if(cur_pos-n>deque_ptr->length||cur_pos-n<0)throw invalid_iterator();

                if(cur_pos-n==deque_ptr->length){
                    return const_iterator(nullptr, nullptr,deque_ptr->length,deque_ptr,been_invalid);
                }

                const_iterator iterator_mid;
                iterator_mid.deque_ptr=deque_ptr;
                iterator_mid.cur_pos=cur_pos-n;
                int offset_;
                iterator_mid.cur_block=deque_ptr->find_block(iterator_mid.cur_pos,offset_);
                block* block_mid=iterator_mid.cur_block;
                iterator_mid.cur_node=block_mid->find_node(offset_);
                return  iterator_mid;
            }
            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const {
                //TODO
                if(deque_ptr!=rhs.deque_ptr)throw invalid_iterator();
                return cur_pos-rhs.cur_pos;
            }
            const_iterator& operator+=(const int &n) {
                //TODO
                if(cur_pos+n>deque_ptr->length||cur_pos+n<0)throw invalid_iterator();
                *this=(*this)+n;
                return *this;
            }
            const_iterator& operator-=(const int &n) {
                //TODO
                if(cur_pos-n>deque_ptr->length||cur_pos-n<0)throw invalid_iterator();
                *this=(*this)-n;
                return *this;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {//there may be some problem
                const_iterator ite_mid=*this;
                *this=*this+1;
                return ite_mid;
                //ite_mid=*this+1;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                *this=*this+1;
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {//报个寂寞的错
                const_iterator ite_mid=*this;
                *this=*this-1;
                return ite_mid;
                //return *this;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                *this=*this-1;
                return *this;
            }
            /**
             * TODO *it
             * 		throw if iterator is invalid//
             *///
            T& operator*() const {
                if(cur_pos<0||cur_pos>=deque_ptr->length||been_invalid)throw invalid_iterator();
                return *(cur_node->value);
            }
            /**
             * TODO it->field
             * 		throw if iterator is invalid
             */
            T* operator->() const noexcept {
                if(cur_pos<0||cur_pos>=deque_ptr->length||been_invalid)throw invalid_iterator();
                return (cur_node->value);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const const_iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return false;
                return true;
            }
            bool operator==(const iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return false;
                return true;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const const_iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return true;
                return false;
            }
            bool operator!=(const iterator &rhs) const {
                if(deque_ptr!=rhs.deque_ptr||cur_pos!=rhs.cur_pos)return true;
                return false;
            }

            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
        };
        /**
         * TODO Constructors
         */
        deque()
        {
            blocks_num=1;
            length=0;//总长度
            head_block= new block(0, nullptr, nullptr, nullptr, nullptr);
            tail_block=head_block;
        }

        void add_block_bottom(block& other)
        {
           // std::cout<<"jinru  ";

            if(blocks_num==0)
            {
                blocks_num=1;
                length=other.nodes_num;
                head_block= new block(other);
                tail_block=head_block;
                return;
            }


            if(blocks_num==1)
            {
               // std::cout<<"jinru  ";
                tail_block->next_block=new block(other);
                tail_block=tail_block->next_block;
                tail_block->pre_block=head_block;
                head_block->next_block=tail_block;
                blocks_num++;
                length+=tail_block->nodes_num;
                return;
            }

            tail_block->next_block=new block(other);
            tail_block->next_block->pre_block=tail_block;
            tail_block=tail_block->next_block;
            blocks_num++;
            length+=tail_block->nodes_num;
        }

        block*  find_block(const int pos,int& node_offset)const//pos>=0
        {

           // std::cout<<"fing_block620    ";
           if(pos>=length)throw index_out_of_bound();
            if(pos==0){
                node_offset=0;
                return head_block;
            }
            if(pos==length-1){
                //node_offset=tail_block->nodes_num-1;
                node_offset=tail_block->nodes_num-1;
                return tail_block;
            }
            int pos_mid=pos;
            block*  block_mid=head_block;
            while(pos_mid>=block_mid->nodes_num)
            {
                pos_mid-=block_mid->nodes_num;
                if(block_mid->next_block== nullptr){break;}
                block_mid=block_mid->next_block;
            }
            node_offset=pos_mid;
            return block_mid;
        }

        node*  find_node_2(int pos){



        }


        deque(const deque &other) //为什么不建议添加explicit?   气抖冷
         {
            //std::cout<<"   copygouzhao   "<<std::endl;
            block* mid_block=other.head_block;
             //std::cout<<"   copy_block   "<<std::endl;
             //std::cout<<this->blocks_num<<"==block_num";
            while(mid_block!= nullptr)
            {
               // std::cout<<"bottom_front  "<<std::endl;
                add_block_bottom(*mid_block);

                //std::cout<<"bottom_back  "<<std::endl;
                mid_block=mid_block->next_block;
            }

        }
        /**
         * TODO Deconstructor
         */
        ~deque() {
            block* mid_block=head_block;
           // while (head_block!= nullptr)
            //{
                //block* mid_mid=mid_block;
              //  mid_block=head_block;
                //mid_block=mid_block->next_block;
                //head_block=head_block->next_block;
                //delete mid_block;
            //}
            for(int i=0;i<blocks_num;i++)
            {
                mid_block=head_block;
                head_block=head_block->next_block;
                delete mid_block;
            }
        }
        /**
         * TODO assignment operator
         */
        deque &operator=(const deque &other) {
            if(this==&other)return *this;
            while (head_block!= nullptr)
            {
                block*mid_block=head_block;
                head_block=head_block->next_block;
                delete mid_block;
            }
            length=0;blocks_num=0;head_block= nullptr;tail_block= nullptr;
            block* block_nimabisi=other.head_block;
          //  node* mid_node=other.head_block;
          //node* mid_node=block_nimabisi->head_node;at693
            while (block_nimabisi!= nullptr)
            {
                add_block_bottom(*block_nimabisi);
                block_nimabisi=block_nimabisi->next_block;
            }
            return *this;
        }
        /**
         * access specified element with bounds checking
         * throw index_out_of_bound if out of bound.
         */
        T & at(const size_t &pos) {//因为这里返回的是引用  而引用的不能是右值
            //std::cout<<"at713   "<<std::endl;
            if(pos>=length||pos<0)throw index_out_of_bound();
            if(pos==length-1){
                T  * value_mid=tail_block->tail_node->value;
                return * value_mid;
            }
            int offset_;
            block* block_mid=find_block(pos,offset_);
            node* node_mid=block_mid->find_node(offset_);
            T *value_mid=(node_mid->value);
           // std::cout<<"   value"<<value_mid;
            return *value_mid;//猜想  为何可以在这里输出  却不能在外面输出  因为  这个函数结束后 或者这一段函数们结束后  值被delete了 然后访问越界 所以这样
            //没错  当我把value new 出来之后 就可以了  日麻
            //T* value=new T(5);
            //return *value;
        }
        const T & at(const size_t &pos) const {//any  difference?
           // std::cout<<"at701   "<<std::endl;
            if(pos>=length||pos<0)throw index_out_of_bound();
            if(pos==length-1){
                T  * value_mid=tail_block->tail_node->value;
                return * value_mid;
            }
            int offset_;
            int pos_mid=pos;
            block* block_mid=find_block(pos,offset_);
            node* node_mid=block_mid->find_node(offset_);
            T* value_mid=(node_mid->value);
            return* value_mid;
        }
        T & operator[](const size_t &pos) {
           // std::cout<<"[]7115"<<std::endl;
            if(pos>=length||pos<0)throw index_out_of_bound();

            if(pos==length-1){
              T  * value_mid=tail_block->tail_node->value;
                return * value_mid;
            }
            int offset_;
            block* block_mid=find_block(pos,offset_);
            node* node_mid=block_mid->find_node(offset_);
            T* value_mid=(node_mid->value);
            return* value_mid;
        }
        const T & operator[](const size_t &pos) const {

          //  std::cout<<"??????"<<std::endl;

            if(pos>=length||pos<0)throw index_out_of_bound();

           // return this->at(pos);
            if(pos==length-1){
                T  * value_mid=tail_block->tail_node->value;
                return * value_mid;
            }
            int offset_;
            block* block_mid=find_block(pos,offset_);
            node* node_mid=block_mid->find_node(offset_);
            T* value_mid=(node_mid->value);
            return* value_mid;
        }
        /**
         * access the first element
         * throw container_is_empty when the container is empty.
         */
        const T & front() const {
            if(head_block->nodes_num==0)throw container_is_empty();
            node* node_mid=head_block->head_node;
            return *(node_mid->value);
        }
        /**
         * access the last element
         * throw container_is_empty when the container is empty.
         */
        const T & back() const {
            if(head_block->nodes_num==0)throw container_is_empty();
            node* node_mid=tail_block->tail_node;
            return *(node_mid->value);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(head_block->head_node,head_block,0, this);
        }
        const_iterator cbegin() const {
            return  const_iterator(head_block->head_node,head_block,0, this);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(nullptr, nullptr,length,this);
        }
        const_iterator cend() const {
            return const_iterator(nullptr, nullptr,length,this);
        }
        /**
         * checks whether the container is empty.
         */
        bool empty() const {
            if(length==0)return true;
            else return false;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return length;
        }
        /**
         * clears the contents
         */
        void clear() {

            block* block_mid=head_block;


            for(int i=0;i<blocks_num;i++){
              block_mid=head_block;
                if(head_block== nullptr)break;
              head_block=head_block->next_block;
              delete block_mid;
            }

            blocks_num=1;length=0;
            head_block=new block(0, nullptr, nullptr, nullptr, nullptr);
            tail_block=head_block;
        }
        /**
         * inserts elements at the specified locat on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator pos, const T &value) {

            if(pos.been_invalid)throw invalid_iterator();

            if(pos.cur_pos<0||pos.cur_pos>length)throw invalid_iterator();
            if(pos.deque_ptr!=this)throw invalid_iterator();

            if(pos.cur_pos==length)
            {
                node* node_mid=new node(value);
                tail_block->insert_node(*this,node_mid,tail_block->nodes_num);
                return iterator(tail_block->tail_node,tail_block,length-1,this);
            }

            int offset_;
            block* block_mid=find_block(pos.cur_pos,offset_);
            node* node_mid=new node(value);
            block_mid->insert_node(*this,node_mid,offset_);
            block_mid=find_block(pos.cur_pos,offset_);
            node_mid=block_mid->find_node(offset_);
            return iterator(node_mid,block_mid,pos.cur_pos,this);
        }
        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos) {//这里可能有问题

            if(pos.been_invalid)throw invalid_iterator();

            if(length==0)throw container_is_empty();


            if(this!=pos.deque_ptr)throw invalid_iterator();


            if(pos.cur_pos<0||pos.cur_pos>=length)throw invalid_iterator();

            if(pos.cur_pos>=length-1){
                pop_back();
                return end();
            }

            int offset_;
            block* block_mid=find_block(pos.cur_pos,offset_);
            block_mid->delete_node(*this,offset_);
            block_mid=find_block(pos.cur_pos,offset_);
            node* node_mid=block_mid->find_node(offset_);
            return iterator(node_mid,block_mid,pos.cur_pos,this);
        }
        /**
         * adds an element to the end
         */
        void push_back(const T &value) {
            tail_block->insert_node(*this,new node(value),tail_block->nodes_num);
        }
        /**
         * removes the last element
         *     throw when the container is empty.
         */
        void pop_back()
        {///todo


            if(length==0){
                throw container_is_empty();
            }

            while(tail_block->nodes_num==0)
            {
                block* block_mid=tail_block;
                tail_block=tail_block->pre_block;
                tail_block->next_block= nullptr;
                delete block_mid;
                blocks_num--;
                if(blocks_num==1)break;
            }

            if(tail_block->nodes_num==1)
            {
                delete tail_block->tail_node;
                 length--;
                tail_block->nodes_num--;
                if(blocks_num==1){
                    return;
                }
                block* block_mid=tail_block;
                tail_block=tail_block->pre_block;
                delete block_mid;
                tail_block->next_block= nullptr;
                blocks_num--;
                return;
            }




            node* node_mid=tail_block->tail_node;
            tail_block->tail_node=node_mid->pre_node;
            delete node_mid;
            tail_block->tail_node->next_node= nullptr;
            tail_block->nodes_num--;
            length--;


        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value) {
            head_block->insert_node(*this,new node(value),0);
        }

        void pop_front() {
            //head_block->delete_node(*this,0);

            if(length==0){
                throw container_is_empty();
            }

            if(head_block->nodes_num==0)
            {
                block* block_mid=head_block;
                head_block=head_block->next_block;
                delete block_mid;
                blocks_num--;
            }

            if(head_block->nodes_num==1)
            {
                delete head_block->head_node;
               length--;
                head_block->nodes_num--;

                if(blocks_num==1){
                    return;
                }
                block* block_mid=head_block;
                head_block=head_block->next_block;
                delete block_mid;
                blocks_num--;
                return;
            }

            node* node_mid=head_block->head_node;
            head_block->head_node=node_mid->next_node;
            delete node_mid;
            head_block->nodes_num--;
            if(length==99999)length=99998;
            else {
               length--;

            }


        }
    };

}

#endif
