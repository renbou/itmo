package markup;

import java.util.List;

/**
 * A single item of an ordered or unordered list
 *
 * Due to the fact that even a single item of a list might be a whole different document,
 * in the sense that this list could be split into a couple of items each of which constitute
 * to the bigger document meaning (for example a theorem's proof might be split into 2 different
 * big parts, each of which could technically be formatted as their own specific document), it is logical
 * for a list item to extend the markup document class, but add extra formatting for turning the document
 * into an item of whatever kind of list by adding the required document format-dependent marks;
 */
public class ListItem extends MarkupDocument {
    public ListItem(List<MarkupBlock> blocks) {
        super(blocks);
    }
    public ListItem(MarkupBlock block) {
        super(List.of(block));
    }

    /**
     * A constructor for creating a list item based on an already created markup document,
     * which might be useful, for example, for writing a document first into a file,
     * and then creating a list of a couple of documents as a separate bigger document.
     *
     * @param document the document which is to be turned into an item of a list
     */
    public ListItem(MarkupDocument document) {
        super(document);
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        sb.append("- ");
        super.toMarkdown(sb);
    }

    @Override
    public void toBBCode(StringBuilder sb) {
        sb.append("[*]");
        super.toBBCode(sb);
    }
}
