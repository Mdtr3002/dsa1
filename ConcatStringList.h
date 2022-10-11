#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

class ConcatStringList {
public:
    class ReferencesList; // forward declaration
    class DeleteStringList; // forward declaration

public:
    static ReferencesList refList;
    static DeleteStringList delStrList;
    class CharAlNode;
    CharAlNode* head;
    CharAlNode* tail;
    int len = 0;
    
    class CharAlNode {
    public:
        string charArrayList;
        CharAlNode* next;
        int count = 0;
        CharAlNode(const char* s) {
            charArrayList = s;
            count = charArrayList.length();
            next = NULL;
            // count = string(s).size();
            // charArrayList = new char[count + 1];
            // for(int i=0;i<count;i++) {
            //     charArrayList[i] = s[i];
            // }
            // charArrayList[count] = '\0';
        }
        ~CharAlNode() {
            count = 0;
            next = nullptr;
            charArrayList="";
        }
    };
    class RefItem {
        public:
        CharAlNode* node;
        int ref = 0;
        RefItem* next;
        RefItem(CharAlNode* n, int upd) {
            node = n;
            ref += upd;
        }
        ~RefItem() {
            ref = 0;
            next = NULL;
        }
    };
    // TODO: may provide some attributes

public:
    ConcatStringList() {
        head = nullptr;
        tail = nullptr;
    }
    ConcatStringList(const char *s) {
        CharAlNode* node = new CharAlNode(s);
        len+=node->count;
        tail = head = node;
        refList.updateList(node, 2);
    };
    int length() const {
        return len;
    };
    char get(int index) const {
        if (index >= len) throw out_of_range("Index of string is invalid!");
        if (index < 0) index = 0;
        CharAlNode* cur = head;
        int idx = index;
        while(idx >= cur->count && cur!=NULL) {
            idx -= cur->count;
            cur = cur->next;
        }
        return (cur->charArrayList)[idx];
    };
    int indexOf(char c) const {
        CharAlNode* cur = head;
        int idx = 0;
        while(cur) {
            // for(int i=0;i<cur->count;i++) {
            //     if((cur->charArrayList)[i]==c) return i+idx;
            // }
            if (cur->charArrayList.find(c) != string::npos)
            {
                return idx + (int)(cur->charArrayList).find(c);  
            }
            idx+=cur->count;
            cur=cur->next;
        }
        return -1;
    };
    void addNode(string s) {
        if(!head) {
            CharAlNode* node = new CharAlNode(s.c_str());
            len+=node->count;
            tail = head = node;
            refList.updateList(node, 2);
        } else {
            CharAlNode* node = new CharAlNode(s.c_str());
            CharAlNode* chk = tail;
            tail->next = node;
            tail = tail->next;
            len+=node->count;
            tail->next = nullptr;
            if(chk == head) {
                refList.updateList(head, -1);
                refList.updateList(tail, 1);
            } else refList.updateTail(chk, tail);
        }
    }
    std::string toString() const {
        string str="";
        CharAlNode* cur = head;
        while(cur) {
            string cArr = cur->charArrayList;
            str+= cArr;
            cur = cur->next;
        }
        return str;
    };
    ConcatStringList concat(const ConcatStringList & otherS) {
        ConcatStringList* sNew = new ConcatStringList();
        sNew->head = head;
        refList.updateList(head, 1);
        sNew->tail = otherS.tail;
        refList.updateList(sNew->tail, 1);
        sNew->len = len + otherS.len;
        tail->next = otherS.head;
        return *sNew;
    };
    ConcatStringList subString(int from, int to) const {
        if((from<0) || (to > len))
        throw out_of_range("Index of string is invalid!");
        if(from >= to)
        throw logic_error("Invalid range");
        CharAlNode* cur = head;
        string str="";
        int curpos = 0;
        ConcatStringList* res = new ConcatStringList();
        while(cur) {
            string chstr = cur->charArrayList;
            int curdix = curpos + chstr.length() - 1;
            if(curdix<from) {}
            else if(curpos <= from && curdix < to) {
                str = (chstr).substr(from-curpos);
                res->addNode(str);
            } else if(curpos <= from && curdix >= to) {
                str = (chstr).substr(from-curpos,to-from);
                res->addNode(str);
                break;
            } else if(curpos >= from && curdix < to) {
                res->addNode(cur->charArrayList);
            } else if(curpos >= from && curdix >= to) {
                str = (chstr).substr(0,to-curpos);
                res->addNode(str);
                break;
            }
            curpos+=cur->count;
            cur = cur->next;
        }
        
        return *res;
    };
    ConcatStringList reverse() const {
        CharAlNode* cur = head;
        ConcatStringList* res = new ConcatStringList();
        if(!cur) return nullptr;
        string rev = "";
        string str = cur->charArrayList;
        int slen = str.length();
        for(int i=(slen-1);i>=0;i--) {
            rev+=str[i];
        }
        cur=cur->next;
        CharAlNode* node = new CharAlNode(rev.c_str());
        res->tail = res->head = node;
        res->len = len;
        if(cur == nullptr) {
            refList.updateList(res->head,2);
        }
        while(cur) {
            rev="";
            string str = cur->charArrayList;
            int slen = str.length();
            for(int i=(slen-1);i>=0;i--) {
                rev+=str[i];
            }
            CharAlNode* newH = new CharAlNode(rev.c_str());
            CharAlNode* chk = res->head;
            newH->next = res->head;
            res->head = newH;
            if(chk == res->tail) {
                refList.updateList(res->head, 1);
                refList.updateList(res->tail, 1);
            } else refList.updateTail(chk, res->head);
            cur=cur->next;
        }
        return *res;
    };
    ~ConcatStringList() {
        bool chk = false;
        len = 0;
        if(head==tail) {
            refList.addDel(head,head);
            refList.updateList(head,-2);
        }
        else {
            refList.addDel(head, tail);
            refList.updateList(head,-1);
            refList.updateList(tail,-1);
        }
        delStrList.checkDel();
        refList.delRef();
    };

public:
    class ReferencesList {
        // TODO: may provide some attributes
        
        public:
            int refLen = 0;
            RefItem* head;
            RefItem* tail;
            ReferencesList() {
                head = tail =nullptr;
            }
            ~ReferencesList()
            {
                RefItem* curr = head;
                while (curr && curr != tail->next)
                {
                    curr = curr->next;
                    delete head;
                    head = curr;
                }
            }
            void bubbleSort() {
                int i, j, swapped;
                for (i = 0; i < refLen; i++) {
                    RefItem* p1 = head;
                    swapped = 0;
                    RefItem* prev = NULL;
                    while (p1->next && p1) {
                        RefItem* p2 = p1->next;
                        if (((p1->ref > p2->ref) && p2->ref!=0) || p1->ref==0) {
                            p1->next = p2->next;
                            p2->next = p1;
                            if(prev) prev->next = p2;
                            else {
                                head = p2;
                            }
                            prev = p2;
                            swapped = 1;
                        } else {
                            prev = p1;
                            p1 = p1->next;
                        }
                    }
                    if (swapped == 0)
                        break;
                }
                RefItem* h = head;
                while(h->next) {
                    h = h->next;
                }
                tail = h;
            }
            void updateList(CharAlNode* node, int count) {
                if(!head) {
                    RefItem* nNode = new RefItem(node, count);
                    head = tail = nNode;
                    refLen = 1;
                } else {
                    RefItem* cur = head;
                    while(cur) {
                        if(cur->node == node) {
                            updateCount(cur,count);
                            return;
                        }
                        cur = cur->next;
                    }
                    RefItem* nNode = new RefItem(node, count);
                    tail->next = nNode;
                    tail = tail->next;
                    if(refLen == 1) head->next = tail;
                    tail->next = nullptr;
                    refLen++;
                    bubbleSort();
                }
            }
            void updateCount(RefItem* item, int nValue) {
                item->ref += nValue;
                bubbleSort();
            }
            int size() const {
                return refLen;
            }
            void updateTail(CharAlNode* old, CharAlNode* nw) {
                RefItem* cur = head;
                while(cur) {
                    if(cur->node == old) {
                        cur->node = nw;
                        return;
                    }
                    cur = cur->next;
                }
            }
            void addDel(CharAlNode* h, CharAlNode* t) {
                if(h==t) {
                    RefItem* cur = head;
                    while(cur) {
                        if(cur->node == h) {
                            delStrList.updateList(cur,cur);
                        }
                        cur = cur->next;
                    }
                } else {
                    RefItem* hItem;
                    RefItem* tItem;
                    RefItem* cur = head;
                    while(cur) {
                        if(cur->node == h) {
                            hItem = cur;
                        }
                        if(cur->node == t) tItem = cur;
                        cur = cur->next;
                    }
                    delStrList.updateList(hItem,tItem);
                }
            }
            void delRef() {
                if(!head) return;
                RefItem* cur = head;
                if(cur->ref != 0) return;
                RefItem* nxt = NULL;
                while(cur) {
                    nxt = cur->next;
                    delete cur;
                    cur = nxt;
                }
                head = tail = nullptr;
                refLen = 0;
            }
            int refCountAt(int index) const {
                if(index>(refLen - 1) || index < 0) throw out_of_range("Index of references list is invalid!");
                if(index == 0) return head->ref;
                int count = 0;
                RefItem* cur = head;
                while(cur && count!=index) {
                    cur=cur->next;
                    count++;
                }
                return head->ref;
            };
            std::string refCountsString() const {
                string res = "RefCounts[";
                RefItem* cur = head;
                while(cur) {
                    res = res + to_string(cur->ref) + ',';
                    cur = cur->next;
                }
                if(head)
                res.pop_back();
                res+="]";
                return res;
            };
    };

    class DeleteStringList {
        // TODO: may provide some attributes
        public:
            class DelItem {
                public:
                RefItem* head;
                RefItem* tail;
                DelItem* next;
                DelItem(RefItem* h, RefItem* t) {
                    head = h;
                    tail = t;
                }
                ~DelItem() {
                    next = nullptr;
                }
            };
            int DelLen = 0;
            DelItem* head;
            DelItem* tail;
            DeleteStringList() {
                head = tail =nullptr;
            }
        public:
            void updateList(RefItem* nodeH, RefItem* nodeT) {
                if(!head) {
                    DelItem* nNode = new DelItem(nodeH, nodeT);
                    head = tail = nNode;
                    DelLen = 1;
                } else {
                    DelItem* nNode = new DelItem(nodeH, nodeT);
                    tail->next = nNode;
                    tail = tail->next;
                    if(DelLen == 1) head->next = tail;
                    tail->next = nullptr;
                    DelLen++;
                }
            }
            void checkDel() {
                if(!head) return;
                DelItem* cur = head;
                while(cur) {
                    if((cur->head->ref + cur->tail->ref) == 0) {
                        DelItem* del = cur;
                        cur = cur->next;
                        delNode(del);
                    } 
                    else cur = cur->next;
                }
            }
            void delNode(DelItem* item) {
                if(item->head->node !=NULL && item->tail->node != NULL) {
                    CharAlNode* cur = item->head->node->next;
                    if(cur!=item->tail->node && item->head->node != item->tail->node) {
                        CharAlNode* nxt = NULL;
                        while(cur->next) {
                            nxt = cur->next;
                            delete cur;
                            cur = nxt;
                        }
                    }
                    removeItem(item);
                } else return;
            }
            void removeItem(DelItem* item) {
               if(!head) return;
               if(head == item) {
                  DelItem* del = head;
                  head = head->next;
                  delete del;
                  DelLen--;
                   return;
               }
               DelItem* cur = head;
               while(cur->next) {
                   if(cur->next == item) {
                       DelItem* del = cur->next;
                       cur->next = cur->next->next;
                       if(tail == del) {
                           tail = cur;
                       }
                       delete del;
                       DelLen--;
                       return;
                   }
                   cur = cur->next;
               }
            }
            bool checkN(CharAlNode* h) {
                DelItem* cur = head;
                while(cur) {
                    if(cur->head->node == h) {
                        return true;
                    }
                    cur=cur->next;
                }
                return false;
            }
            int size() const {
                return DelLen;
            };
            std::string totalRefCountsString() const {
                string res = "TotalRefCounts[";
                DelItem* cur = head;
                int total;
                while(cur) {
                    if(cur->head == cur->tail) total = cur->head->ref;
                    else total = cur->head->ref + cur->tail->ref;
                    res = res + to_string(total) + ',';
                    cur = cur->next;
                }
                if(head)
                res.pop_back();
                res+="]";
                return res;
            };
    };
};

#endif // __CONCAT_STRING_LIST_H__