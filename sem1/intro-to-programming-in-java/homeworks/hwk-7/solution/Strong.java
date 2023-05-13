package markup;

import java.util.List;

/**
 * Style element which defines elements stylized as bold and strong
 */
public class Strong extends AbstractMarkupStyle {
    public Strong(List<MarkupItem> elements) {
        super(elements);
    }

    @Override
    public void beginMarkdownStyle(StringBuilder sb) {
        sb.append("__");
    }

    @Override
    public void endMarkdownStyle(StringBuilder sb) {
        sb.append("__");
    }

    @Override
    public void beginBBCodeStyle(StringBuilder sb) {
        sb.append("[b]");
    }

    @Override
    public void endBBCodeStyle(StringBuilder sb) {
        sb.append("[/b]");
    }
}
