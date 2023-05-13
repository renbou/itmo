package markup;

import java.util.List;

public class MarkupTest {
    public static void main(String[] args) {
        UnorderedList l = new UnorderedList(new ListItem(
                new Paragraph(List.of(new Text("a")))
        ));
        StringBuilder res = new StringBuilder();
        l.toBBCode(res);
        System.out.println(res.toString());
    }
}
