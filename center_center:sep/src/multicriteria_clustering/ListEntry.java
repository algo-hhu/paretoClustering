package multicriteria_clustering;

public class ListEntry <T>{
    private ListEntry<T> next;
    private ListEntry<T> prev;
    private final T value;
    DLList<T> list;

    ListEntry<T> get_next(){
        return next;
    }

    ListEntry<T> get_prev(){
        return prev;
    }

    ListEntry(T entry, ListEntry<T> predecessor, DLList<T> l){
        next = null;
        prev = predecessor;
        value = entry;
        list = l;
    }

    void set_next(ListEntry<T> new_next){
        next = new_next;
    }

    void set_prev(ListEntry<T> new_prev){
        prev = new_prev;
    }

    void remove_entry(){
        if(prev != null){
            prev.set_next(next);
        }
        else{
            list.set_start(next);
        }
        if(next != null){
            next.set_prev(prev);
        }
        else{
            list.set_end(prev);
        }
        list.decrease_length();
    }

    T get_value(){
        return value;
    }
}
