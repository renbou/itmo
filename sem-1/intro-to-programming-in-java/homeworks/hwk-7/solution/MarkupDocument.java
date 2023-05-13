package markup;

import java.util.LinkedList;
import java.util.List;

/**
 * A logically and structurally valid markup document.
 *
 * This fits as the most top-level markup element, which basically contains
 * all the resulting document's blocks inside of it, no matter their meaning.
 * As such, an instance of this class is simply constructed from a list of all the
 * required blocks and can then be converted to the required format, whatever it might be.
 *
 * Even though the Markup interface says that we should create immutable object, it makes sense
 * for this to be mutable, allowing us to first initialize the document with some needed blocks,
 * and then append more blocks as we work through the data.
 */
public class MarkupDocument implements Markup {
    protected List<MarkupBlock> blocks;

    public MarkupDocument(List<MarkupBlock> blocks) {
        this.blocks = new LinkedList<>(blocks);
    }

    /**
     * A maybe stupid constructor for creating a new document based on a different document.
     * However, this might actually be useful when you want to write a couple of documents
     * which all contain the same introductory part, so you create that part as a document
     * and then initialize this document with it, appending new elements afterwards.
     *
     * @param document the document from which to initialize this one
     */
    public MarkupDocument(MarkupDocument document) {
        blocks = document.blocks;
    }

    @Override
    public void toMarkdown(StringBuilder sb) {
        for (MarkupBlock block : blocks) {
            block.toMarkdown(sb);
        }
    }

    @Override
    public void toBBCode(StringBuilder sb) {
        for (MarkupBlock block : blocks) {
            block.toBBCode(sb);
        }
    }

    /**
     * Method which appends a new markup block to this document's end, without returning a new copy
     * or anything of that kind, because we said that MarkupDocument is mutable. Blocks.add should
     * always return true, because the backing list is a LinkedList, so there's no point in returning
     * anything.
     *
     * @param block the block to add to the end of the document
     */
    public void addBlock(MarkupBlock block) {
        this.blocks.add(block);
    }
}
