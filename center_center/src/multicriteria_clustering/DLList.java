package multicriteria_clustering;

public class DLList<T> {
    private int length;
    private ListEntry<T> start;
    private ListEntry<T> end;

    DLList(){
        length = 0;
        start = null;
        end = null;
    }

    ListEntry<T> get_start(){
        return start;
    }
    void set_start(ListEntry<T> entry){
        start = entry;
    }

    void set_end(ListEntry<T> entry){
        end = entry;
    }

    int size(){
        return length;
    }

    void decrease_length(){
        length--;
    }

   T top(){
        return start.get_value();
    }

    ListEntry<T> push(T value){
        ListEntry<T> new_entry = new ListEntry<T>(value, end, this);
        if(end != null){
            end.set_next(new_entry);
        }
        else{
            start = new_entry;
        }
        length++;
        end = new_entry;
        return new_entry;
    }
}
