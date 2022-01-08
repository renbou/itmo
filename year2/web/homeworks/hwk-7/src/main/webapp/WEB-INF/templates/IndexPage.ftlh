<#import "commons.ftlh" as c>
<#import "templates.ftlh" as t>

<@c.page>
    <div id="articles" class="articles">
        <@t.articleTemplate/>
        <div class="noData"></div>
    </div>
    <script>
        $(function () {
            const articlesTemplate = new template.Template("#articles template", {
                    ".title": "title",
                    ".information": (data) => "By " + data["user"]["login"],
                    ".body p": "text",
                    ".footer .right span": "creationTime"
                }, "#articles"
            )

            const $noData = $(".noData");
            $noData.text("Loading...");

            new requests.Request("post article/findAll", {},
                (response) => {
                    if (response["articles"] && response["articles"].length > 0) {
                        $noData.hide();
                        articlesTemplate.format(response["articles"]);
                    } else {
                        $noData.text("Welcome to Codeforces!");
                    }
                }
            ).execute(this)
        })
    </script>
</@c.page>
