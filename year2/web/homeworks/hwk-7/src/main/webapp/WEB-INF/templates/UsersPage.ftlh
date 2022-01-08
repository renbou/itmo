<#-- @ftlvariable name="users" type="ru.itmo.wp.model.domain.User[]" -->
<#-- @ftlvariable name="user" type="ru.itmo.wp.model.domain.User" -->
<#import "commons.ftlh" as c>

<@c.page>
    <div id="users" class="datatable">
        <div class="caption">User</div>
        <table>
            <thead>
            <tr>
                <th>Id</th>
                <th>Login</th>
                <#if user?? && user.admin>
                    <th>Admin</th>
                </#if>
            </tr>
            </thead>
            <tbody>
            <template>
                <tr>
                    <td class="user-id"></td>
                    <td class="user_login"></td>
                    <#if user?? && user.admin>
                        <td>
                            <div class="user-admin"></div>
                            <a href="#" class="toggleAdmin"></a>
                        </td>
                    </#if>
                </tr>
            </template>
            <tr class="noData">
                <td colspan="3"></td>
            </tr>
            </tbody>
        </table>
    </div>

    <script>
        $(function () {
            let usersTemplateFormat = {
                ".user-id": "id",
                ".user_login": "login",
                "tr": {
                    "attributes": {
                        "data-userId": "id"
                    }
                }
            };
            <#if user?? && user.admin>
                usersTemplateFormat[".user-admin"] = "admin";
                usersTemplateFormat[".toggleAdmin"] = (data) => data["admin"] ? "disable" : "enable";
            </#if>

            const usersTemplate = new template.Template(
                "#users template", usersTemplateFormat, "#users tbody"
            );
            const $noData = $("#users .noData");
            $noData.find("td").text("Loading...");

            new requests.Request("post findAll", {},
                (response) => {
                    if (response["users"]) {
                        $noData.hide();
                        usersTemplate.format(response["users"]);
                    } else {
                        $noData.find("td").text("No data");
                    }
                    $("#users tr").click(requests.Run(new requests.Request(
                        "post findUser",
                        {
                            userId: vals.dataAttributeValue
                        },
                        (response) => notify(response["user"].creationTime)
                    )));
                    $("#users td a.toggleAdmin").click(requests.Run(new requests.Request(
                        "post toggleAdmin",
                        {
                            userId: (element) => $cached(element).parent().parent().attr("data-userId")
                        },
                        function() {
                            toggleText(this, ["enable", "disable"]);
                            const userAdminDiv = this.parent().find(".user-admin");
                            toggleText(userAdminDiv, ["true", "false"]);
                        }
                    )))
            }).execute(this);
        })
    </script>
</@c.page>
