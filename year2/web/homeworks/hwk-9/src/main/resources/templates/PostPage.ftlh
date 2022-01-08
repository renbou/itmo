<#-- @ftlvariable name="post" type="ru.itmo.wp.domain.Post" -->
<#-- @ftlvariable name="commentForm" type="ru.itmo.wp.form.CommentText" -->
<#import "commons.ftlh" as c>

<@c.page>
    <#if post??>
        <@c.post post=post withLink=false/>
        <#if user??>
            <div class="comment form-box">
                <div class="header">Write a comment</div>
                <div class="body">
                    <form method="post" action="">
                        <input type="hidden" name="action" value="enter"/>
                        <div class="field">
                            <div class="name">
                                <label for="text">Text</label>
                            </div>
                            <div class="value">
                                <textarea id="text" name="text">${commentForm.text!}</textarea>
                            </div>
                            <@c.error "commentForm.text"/>
                        </div>
                        <div class="button-field">
                            <input type="submit" value="Submit">
                        </div>
                    </form>
                </div>
            </div>
        </#if>
        <div class="articleComments">
            <ul>
                <#list post.comments as comment>
                    <li>
                        ${comment.user.login} at ${comment.creationTime}: ${comment.text}
                    </li>
                </#list>
            </ul>
        </div>
    <#else>
        No such post.
    </#if>
</@c.page>
