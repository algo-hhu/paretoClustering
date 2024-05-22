package multicriteria_clustering;
import java.util.ArrayList;

public class CCGraph {
    ArrayList<DLList<Integer>> blocking;
    ArrayList<DLList<CCEdge>> adjacent;
    int indep_size;

    CCGraph(int size){
        blocking = new ArrayList<DLList<Integer>>();
        adjacent = new ArrayList<DLList<CCEdge>>();
        for(int i = 0; i < size; i++){
            blocking.add(new DLList<Integer>());
            adjacent.add(new DLList<CCEdge>());
        }
        indep_size = size;
    }

    void add_edge(int v, int w){
        CCEdge front = new CCEdge(w);
        CCEdge back = new CCEdge(v);
        if(is_independent(v)){
            Boolean removed = is_independent(w);
            ListEntry<Integer> b = blocking.get(w).push(v);
            front.set_blockade(b);
            if(removed){
                indep_size--;
                ListEntry<CCEdge> current = adjacent.get(w).get_start();
                while(current != null){
                    remove_blockade(current.get_value());
                    current = current.get_next();
                }
            }
        } else if(is_independent(w)){
            ListEntry<Integer> b = blocking.get(v).push(w);
            back.set_blockade(b);
        }
        adjacent.get(v).push(front);
        adjacent.get(w).push(back);
    }


    void remove_edge(int v, int w){
        ListEntry<CCEdge> front = search_edge(v,w);
        ListEntry<CCEdge> back = search_edge(w,v);
        front.remove_entry();
        back.remove_entry();
        if(is_independent(v)){
            remove_blockade(front.get_value());
        } else if (is_independent(w)) {
            remove_blockade(back.get_value());
        }
    }

    void remove_blockade(CCEdge edge){
        int w = edge.target;
        edge.remove_blockade();
        if(is_independent(w)){
            indep_size++;
            ListEntry<CCEdge> current = adjacent.get(w).get_start();
            while(current != null){
                int u = current.get_value().target;
                ListEntry<Integer> new_blockade = blocking.get(u).push(w);
                current.get_value().set_blockade(new_blockade);
                current = current. get_next();
            }
        }
    }

    Boolean is_independent(int v){
        return (blocking.get(v).size() == 0);
    }

    int indep_size(){
        return indep_size;
    }

    Result return_result(){
        Result result = new Result();
        for(int i = 0; i < blocking.size(); i++){
            if(is_independent(i)){
                result.centers.add(i);
                result.assignment.add(i);
            }
            else{
                result.assignment.add(blocking.get(i).top());
            }
        }
        return result;
    }

    ListEntry<CCEdge> search_edge(int v, int w){
        ListEntry<CCEdge> current = adjacent.get(v).get_start();
        boolean found = false;
        while(current != null && !found){
            if(current.get_value().target == w){
                found = true;
            }
            else {
                current = current.get_next();
            }
        }
        return current;
    }
}
