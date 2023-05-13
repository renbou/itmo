package markup;

/**
 * Interface defining an immutable inner item of a markup block
 *
 * These kinds of items by themselves cannot be converted to a MarkupDocument,
 * but there might be times when we want to construct a MarkupBlock consisting of just this one
 * item, so methods for converting these inner items into Paragraphs, which are the most basic
 * block, should be present.
 *
 * As such, all inner items are specified by this interface, and not only differ in their
 * implementation from MarkupBlock's but also syntactically (in the document) and logically.
 */
public interface MarkupItem extends Markup {
    /**
     * Constructs a paragraph block which contains nothing but this one element inside of it,
     * forming a valid MarkupBlock which can be then joined by other blocks to form a document.
     * The paragraph returned by this method should usually contain a reference to the object,
     * allowing the reuse of such objects by multiple paragraphs. This is valid because we agree
     * that all elements must be immutable, and this allows our code to be more memory-friendly.
     *
     * @return a Paragraph which consists only of this single inner item
     */
    Paragraph toParagraph();
}
