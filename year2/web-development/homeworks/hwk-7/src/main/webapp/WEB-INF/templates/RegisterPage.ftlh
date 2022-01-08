<#import "commons.ftlh" as c>

<@c.page>
    <div id="registration" class="form-box">
        <div class="header">Register</div>
        <div class="body">
            <form method="post" action="">
                <input type="hidden" name="action" value="register"/>
                <div class="field">
                    <div class="name">
                        <label for="login">Login</label>
                    </div>
                    <div class="value">
                        <input id="login" name="login"/>
                    </div>
                </div>
                <div class="field">
                    <div class="name">
                        <label for="password">Password</label>
                    </div>
                    <div class="value">
                        <input id="password" name="password" type="password"/>
                    </div>
                </div>
                <div class="error"></div>
                <div class="button-field">
                    <input type="submit" value="Register">
                </div>
            </form>
        </div>
    </div>
    <script>
        $(function () {
            $("#registration form").on("submit", requests.Run(new requests.Request("post register", {
                login: vals.inputValue,
                password: vals.inputValue,
            })));
        })
    </script>
</@c.page>
