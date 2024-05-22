package multicriteria_clustering;

public class CCEdge {
    private ListEntry<Integer> blockade;
    final int target;

    CCEdge(int w){
        target = w;
        blockade = null;
    }

    void remove_blockade(){
        if(blockade != null){
            blockade.remove_entry();
            blockade = null;
        }
    }

    void set_blockade(ListEntry<Integer> new_blockade){
        blockade = new_blockade;
    }


}
