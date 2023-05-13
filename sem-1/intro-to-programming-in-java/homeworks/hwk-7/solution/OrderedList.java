package markup;

import java.util.List;

/**
 * A list which formats the elements with consideration to their order in the list
 */
public class OrderedList extends AbstractMarkupList {
    public OrderedList(List<ListItem> listElements) {
        super(listElements);
    }

    public OrderedList(ListItem... listElements) {
        super(listElements);
    }

    @Override
    public void beginBBCodeList(StringBuilder sb) {
        sb.append("[list=1]");
    }

    @Override
    public void endBBCodeList(StringBuilder sb) {
        sb.append("[/list]");
    }
}
