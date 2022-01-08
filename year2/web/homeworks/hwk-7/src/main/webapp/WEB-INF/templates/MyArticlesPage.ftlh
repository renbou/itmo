<#-- @ftlvariable name="articles" type="ru.itmo.wp.model.domain.Article[]" -->
<#import "commons.ftlh" as c>

<@c.page>
    <div id="articles" class="datatable">
        <div class="caption">My articles</div>
        <table>
            <thead>
            <tr>
                <th>Id</th>
                <th>Title</th>
                <th>Privacy</th>
            </tr>
            </thead>
            <tbody>
            <#list articles as article>
                <tr>
                    <td>${article.id}</td>
                    <td>${article.title}</td>
                    <td>
                        <button class="togglePrivacy" data-articleId="${article.id}">${article.hidden?then("Show", "Hide")}</button>
                    </td>
                </tr>
            </#list>
            </tbody>
        </table>
    </div>

    <script>
        $(function () {
            $("#articles td button.togglePrivacy").click(requests.Run(new requests.Request(
                "post toggleHidden",
                {
                    articleId: vals.dataAttributeValue
                },
                function() {
                    toggleText(this, ["Show", "Hide"]);
                }
            )));
        })
    </script>
</@c.page>
