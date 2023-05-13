package markup;

import java.util.List;

/**
 * Style element which defines elements with an added emphasis to them
 */
public class Emphasis extends AbstractMarkupStyle {
    public Emphasis(List<MarkupItem> elements) {
        super(elements);
    }

    @Override
    public void beginMarkdownStyle(StringBuilder sb) {
        sb.append('*');
    }

    @Override
    public void endMarkdownStyle(StringBuilder sb) {
        sb.append('*');
    }

    @Override
    public void beginBBCodeStyle(StringBuilder sb) {
        sb.append("[i]");
    }

    @Override
    public void endBBCodeStyle(StringBuilder sb) {
        sb.append("[/i]");
    }
}
