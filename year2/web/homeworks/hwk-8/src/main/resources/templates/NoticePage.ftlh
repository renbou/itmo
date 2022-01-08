<#-- @ftlvariable name="noticeForm" type="ru.itmo.wp.lesson8.form.NoticeText" -->
<#import "/spring.ftl" as spring>
<#import "commons.ftlh" as c>

<@c.page>
    <div class="enter form-box">
        <div class="header">Create notice</div>
        <div class="body">
            <form method="post" action="">
                <input type="hidden" name="action" value="enter"/>
                <div class="field">
                    <div class="name">
                        <label for="text">Text</label>
                    </div>
                    <div class="value">
                        <textarea id="text" name="text">${noticeForm.text!}</textarea>
                    </div>
                    <@c.error "noticeForm.text"/>
                </div>
                <div class="button-field">
                    <input type="submit" value="Enter">
                </div>
            </form>
        </div>
    </div>
</@c.page>
