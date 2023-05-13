package markup;

/**
 * Interface defining a basic immutable markup block
 *
 * This defines all classes which syntactically and logically can be converted to
 * and used as a separate document, with no extra formatting required. Examples
 * of this are a paragraph, lists, blockquote's. They differ from inner markup items
 * in the sense that they alone already form a logically valid document, which doesn't hold
 * true for elements like an emphasized word.
 */
public interface MarkupBlock extends Markup {
    /**
     * Constructs a valid MarkupDocument which doesn't contain any blocks other than this one,
     * and which can be immediately converted to any required format. The resulting markup document
     * should usually contain a reference to an already existing object, allowing reuse of markup block objects
     * by multiple documents. This is all good and valid since they are immutable, and allows our code
     * to be more memory-friendly.
     *
     * @return a MarkupDocument which consists only of this single logical block
     */
    MarkupDocument toMarkupDocument();
}
