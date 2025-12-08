# UGameplayAbility

UGameplayAbility下的NetExecutionPolicy模式：它有4个成员变量
| NetExecutionPolicy  | 执行位置                                | 作用   
| ------------------- | ---------------------                   |---------------------
| **Local**           | 只在本地（客户端或服务器）执行，不网络同步 |
| **ServerOnly**      | 只在服务器执行                          |
| **LocalPredicted**  | 客户端执行 + 服务器验证（预测）           |客户端先执行ActivateAbility，然后服务器再ActivateAbility
| **ServerInitiated** | 服务器触发，客户端仅收到结果              |



标签设置:
1. 资产标签（Ability Tags）
    GA 自身的“身份证”，可多个。
    描述该能力自身的身份、类型、分类。
    如：
    Ability.Attack.Melee
    Ability.Skill.Fireball

    AbilityTags为能力的成员变量
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack.Melee")));
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Type.Skill")));


2. 取消带标签的能力（Cancel Abilities With Tags）
    当这个能力激活时，会主动取消所有(资产标签)具有这些标签的能力,一激活就执行一次扫表打断
    如果Cancel Abilities With Tags==Ability.Attack,那么正在进行中、拥有 Ability.Attack 标签的技能，立即被中断并强制 EndAbility。

3. 用标签阻止能力（Block Abilities With Tags）
    当此能力 正在激活 时，它会阻止拥有这些标签(资产标签)的能力尝试激活,任何试图进入激活流程且带匹配标签的 GA（已激活的不受影响）,只要本 GA 还处于 “Active” 就把它们挡在 CanActivate 之外 

4. 激活标签已拥有的能力（Activation Owned Tags）
    当该能力 成功激活后，这些标签被加到角色 ASC 上，能力结束时自动移除。

5. 激活标签所需标签（Activation Required Tags）
    激活能力前，ASC 上 必须拥有这些标签。如果缺少 → 不能激活。

6. 激活阻止标签（Activation Blocked Tags）
    如果角色 ASC 拥有这些标签，则能力 不能激活。

7. 源所需标签（Source Required Tags）
    技能施法者必须拥有这些标签，能力才可以执行其效果。

8. 目标所需标签（Target Required Tags）
    目标必须有这些标签，技能效果才能生效。
    用于：
    ·组合技
    ·特定状态触发效果
9. 目标被阻止标签（Target Blocked Tags）
    如果目标拥有这些标签，则技能效果不会生效。

校验失败时可以手动执行如下函数，表示预测失败时，服务器调用，会触发回滚
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    AbilitySystemComponent->RejectAbility() 等价于 EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);




